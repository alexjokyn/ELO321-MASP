#include "best-fit-tdas.h"


void holes_list_insert(hole_t** hole_ptr, hole_t** head, hole_t** tail){
    hole_t* new_hole = *hole_ptr;
    unsigned int position = new_hole->position;

    if(*head == NULL || *tail == NULL){
        *head = new_hole;
        *tail = new_hole;
        new_hole->next = NULL;
        new_hole->prev = NULL;

        return;
    }
    else{
        hole_t* prev = holes_list_find_closest_hole(head, tail, position);
        hole_t* next = NULL;

        if(prev == NULL){
            next = *head; 
        }
        else{
            next = prev->next;
        }

        short int merged_left= 0;

        // Check and merge to the left
        if(prev != NULL){
            if(prev->size + prev->position == new_hole->position){
                prev->size += new_hole->size;
                merged_left = 1;

                free(new_hole);
                new_hole = prev;
            }
        }
        // Check and merge to the right
        if(next != NULL){
            if(new_hole->position + new_hole->size == next->position){
                new_hole->size += next->size;
                
                new_hole->next = next->next;

                if(next->next != NULL){
                    next->next->prev = new_hole;
                }
                else {
                    *tail = new_hole;
                }

                free(next);

                merged_left = 1;
            }
        }
        // If no merge ocurred
        if(!merged_left){
            if(prev==NULL){
                new_hole->next = *head;
                new_hole->prev = NULL;
                if(*head != NULL){
                    (*head)->prev = new_hole;
                }
                *head = new_hole;
            } 
            else{
                new_hole->prev = prev;
                new_hole->next = next;
                prev->next = new_hole;

                if(next!=NULL){
                    next->prev = new_hole;
                } 
                else{
                    *tail = new_hole;
                }
            }   
        }
    }
}

hole_t* holes_list_find_best_fit(hole_t* head, hole_t* tail, unsigned int requested_size){
    unsigned int best_fit_size = UINT_MAX;
    hole_t* best_fit_hole = NULL;

    hole_t* current = head;

    while(current!=NULL){
        if(current->size >= requested_size){
            if(current->size < best_fit_size){
                best_fit_size = current->size;
                best_fit_hole = current;

                if(current->size == requested_size){
                    return current;
                }
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

void holes_list_requested_hole(hole_t** head, hole_t** tail, unsigned int requested_size){

}
