#include "../include/best-fit-tdas.h"

// Holes
// EN src/best-fit-tdas.c

void holes_list_insert(hole_t** hole_ptr, hole_t** head, hole_t** tail){
    hole_t* new_hole = *hole_ptr;
    unsigned int position = new_hole->position;

    // Empty list
    if(*head == NULL){
        *head = new_hole;
        *tail = new_hole;
        new_hole->next = NULL;
        new_hole->prev = NULL;
        return;
    }

    hole_t* prev = holes_list_find_closest_hole(*head, *tail, position);
    hole_t* next = NULL;
    
    if(prev == NULL){
        // Insert at begin
        next = *head;
        new_hole->next = next;
        new_hole->prev = NULL;
        next->prev = new_hole;
        *head = new_hole;
    } else {
        // Insert after prev
        next = prev->next;
        
        new_hole->prev = prev;
        new_hole->next = next;
        prev->next = new_hole;

        if(next != NULL){
            next->prev = new_hole;
        } else {
            *tail = new_hole;
        }
    }

    // Apply coalescing (fusion) if necessary

    // Coalescing right
    if(new_hole->next != NULL){
        hole_t* right_neighbor = new_hole->next;
        
        if(new_hole->position + new_hole->size == right_neighbor->position){
            // Absorb neighbor
            new_hole->size += right_neighbor->size;
            
            // Remove neighbor
            new_hole->next = right_neighbor->next;
            if(right_neighbor->next != NULL){
                right_neighbor->next->prev = new_hole;
            } else {
                *tail = new_hole; // If the neighbor was the last one 
            }
            
            free(right_neighbor); 
        }
    }

    // Coalescing left
    if(new_hole->prev != NULL){
        hole_t* left_neighbor = new_hole->prev;
        
        if(left_neighbor->position + left_neighbor->size == new_hole->position){
            // Coalesce - the left neighbor will absorb the new hole
            left_neighbor->size += new_hole->size;
            
            // Remove new_hole
            left_neighbor->next = new_hole->next;
            if(new_hole->next != NULL){
                new_hole->next->prev = left_neighbor;
            } else {
                *tail = left_neighbor; // If now left_neighbor is the last at the list
            }
            
            free(new_hole); 
        }
    }
}

hole_t* holes_list_find_best_fit(hole_t* head, hole_t* tail, unsigned int requested_size){
    unsigned int best_fit_size = UINT_MAX;
    hole_t* best_fit_hole = NULL;

    hole_t* current = head;

    while(current!=NULL){
        if(current->size >= requested_size){
            if(current->size == requested_size){
                return current;
            }
            if(current->size < best_fit_size){
                best_fit_size = current->size;
                best_fit_hole = current;
            }
        }
        current = current->next;
    }

    return best_fit_hole;
}

hole_t* holes_list_find_closest_hole(hole_t* head, hole_t* tail, unsigned int position){
    hole_t* current = head;

    if(current == NULL){
        return NULL;
    }

    while(current != NULL){
        if(current->position > position){
            return current->prev;
        }
        current = current->next;
    } 

    return tail;
}

unsigned int holes_list_request_hole(hole_t** head, hole_t** tail, unsigned int requested_size, unsigned int pid, used_position_t** head_used_list, used_position_t** tail_used_list){
    hole_t* found_hole = holes_list_find_best_fit(*head, *tail, requested_size);
    
    if(found_hole == NULL){
        // Añadir a queue de processos por alocar
        return UINT_MAX;
    }
    
    if(found_hole->size == requested_size){ 
        // Delete the hole
        if(found_hole->prev!=NULL){
            found_hole->prev->next = found_hole->next;
        }
        else{
            *head = found_hole->next;
        }
        if(found_hole->next!=NULL){
            found_hole->next->prev = found_hole->prev;
        }
        else{
            *tail = found_hole->prev;
        }

        // Record used position
        used_position_t* new_allocation = malloc(sizeof(used_position_t));
        new_allocation->pid = pid;
        new_allocation->position = found_hole->position;
        new_allocation->size = requested_size;

        used_list_insert(new_allocation, head_used_list, tail_used_list);

        // Delete the hole
        found_hole->prev = NULL;
        found_hole->next = NULL;
        free(found_hole);

        return new_allocation->position;
    }
    else{
        unsigned int alloc_pos = found_hole->position;
        unsigned int difference = found_hole->size - requested_size;

        found_hole->size = difference;
        found_hole->position = found_hole->position + requested_size;
         
        // Record used position
        used_position_t* new_allocation = malloc(sizeof(used_position_t));
        new_allocation->pid = pid;
        new_allocation->position = alloc_pos;
        new_allocation->size = requested_size;

        used_list_insert(new_allocation, head_used_list, tail_used_list);

        return alloc_pos; 
    }

    return UINT_MAX;
}


void holes_list_remove(hole_t* h, hole_t** head, hole_t** tail) {
    if (h->prev != NULL)
        h->prev->next = h->next;
    else
        *head = h->next;

    if (h->next != NULL)
        h->next->prev = h->prev;
    else
        *tail = h->prev;

    h->prev = NULL;
    h->next = NULL;
}



// Used Positions
void used_list_insert(used_position_t* new_alloc, used_position_t** head, used_position_t** tail){
    if(*head == NULL){
        *head = new_alloc;
        *tail = new_alloc;
        new_alloc->prev = NULL;
        new_alloc->next = NULL;
        return;
    }

    used_position_t* prev = used_list_find_closest_position(*head, *tail, new_alloc->position);

    if(prev == NULL){
        new_alloc->next = *head;
        new_alloc->prev = NULL;
        if(*head != NULL){
            (*head)->prev = new_alloc;
        }
        *head = new_alloc;
    }
    else {
        new_alloc->prev = prev;
        new_alloc->next = prev->next;

        prev->next = new_alloc; 

        if(new_alloc->next != NULL){
            new_alloc->next->prev = new_alloc;
        } else {
            *tail = new_alloc; // Si no hay siguiente, somos el nuevo tail
        }
    }
}

used_position_t* used_list_remove(used_position_t** head, used_position_t** tail, unsigned int position){
    used_position_t* to_remove = *head;
    
    while(to_remove != NULL){
        if(to_remove->position == position){
            
            if(to_remove->prev != NULL){
                to_remove->prev->next = to_remove->next;
            } else {
                *head = to_remove->next;
            }

            if(to_remove->next != NULL){
                to_remove->next->prev = to_remove->prev;
            } else {
                *tail = to_remove->prev;
            }

            to_remove->prev = NULL;
            to_remove->next = NULL;
            return to_remove;
        }
        to_remove = to_remove->next;
    }
    return NULL; // No encontrado
}

used_position_t* used_list_find_closest_position(used_position_t* head, used_position_t* tail, unsigned int position){
    used_position_t* current = head;

    // Si la lista está vacía
    if(current == NULL) return NULL;

    while(current != NULL){
        if(current->position > position){
            return current->prev; // Encontramos uno mayor, devolvemos el anterior
        }
        current = current->next;
    }
    
    // Si llegamos aquí, somos el elemento con la posición más grande
    return tail; 
}

// Queue for processes without sufficient memory
