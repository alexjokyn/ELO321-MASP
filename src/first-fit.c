#include "first-fit.h"
#include <stdlib.h>
#include <stdio.h>


extern void holes_list_remove(hole_t* hole_to_remove, hole_t** head, hole_t** tail);
extern void used_list_insert(used_position_t* new_alloc, used_position_t** head, used_position_t** tail);


int holes_list_request_first_fit(
    hole_t** head_holes_list, hole_t** tail_holes_list,
    used_position_t** head_used_list, used_position_t** tail_used_list,
    unsigned int requested_size,
    unsigned int pid
) {
    hole_t* current_hole = *head_holes_list;
    hole_t* found_hole = NULL;

    // 1. Búsqueda First-Fit: encontrar el primer hueco válido
    while (current_hole != NULL) {
        if (current_hole->size >= requested_size) {
            found_hole = current_hole;
            break; 
        }
        // 
        
        current_hole = current_hole->next;
    }

    // 2. Fallo de asignación
    if (found_hole == NULL) {
        return -1; 
    }

    // 3. Asignación exitosa
    unsigned int allocated_position = found_hole->position;
    unsigned int original_hole_size = found_hole->size;
    unsigned int remaining_size = original_hole_size - requested_size;

    // A. Crear la nueva entrada de posición ocupada
    used_position_t* new_used_alloc = (used_position_t*) malloc(sizeof(used_position_t));
    if (new_used_alloc == NULL) {
        return -1; 
    }
    new_used_alloc->pid = pid;
    new_used_alloc->position = allocated_position;
    new_used_alloc->size = requested_size;
    
    new_used_alloc->prev = NULL; 
    new_used_alloc->next = NULL;

    used_list_insert(new_used_alloc, head_used_list, tail_used_list);

    // B. Gestionar el hueco restante
    if (remaining_size == 0) {

        holes_list_remove(found_hole, head_holes_list, tail_holes_list);
        free(found_hole);
        
    } else {
        found_hole->position = allocated_position + requested_size;
        found_hole->size = remaining_size;                          
    }

    return pid;
}