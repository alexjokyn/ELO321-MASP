#include <stdlib.h>

#include "../include/best-fit.h"
#include "../include/best-fit-tdas.h"

static hole_t* holes_head = NULL;
static hole_t* holes_tail = NULL;

static used_position_t* used_head = NULL;
static used_position_t* used_tail = NULL;

static unsigned int pid_pos_map[MAX_PID_MAP];

int best_fit_init(unsigned int total_memory_size){
    holes_head = NULL; holes_tail = NULL;
    used_head = NULL; used_tail = NULL;

    for(int i = 0; i<MAX_PID_MAP; ++i){
        pid_pos_map[i] = UINT_MAX;
    }

    hole_t* initial_hole = malloc(sizeof(hole_t));
    if(initial_hole == NULL){
        return 0;
    }
    initial_hole->position = 0;
    initial_hole->size = total_memory_size;
    initial_hole->prev = NULL;
    initial_hole->next = NULL;

    holes_list_insert(&initial_hole, &holes_head, &holes_tail);

    return 1;
}

int best_fit_alloc(unsigned int pid, unsigned int size){
    if(pid >= MAX_PID_MAP) return -1;

    unsigned int position = holes_list_request_hole(&holes_head, &holes_tail, size, pid, &used_head, &used_tail);

    if(position == UINT_MAX){
        return 0;
    }else{
        pid_pos_map[pid] = position;
        return 1;
    }
}

void best_fit_dealloc(unsigned int pid){
    if(pid >= MAX_PID_MAP) return;
    
    unsigned int position = pid_pos_map[pid];    
    
    if(position == UINT_MAX) return;

    unsigned int free_size = 0;

    used_position_t* removed = used_list_remove(&used_head, &used_tail, position);

    if(removed!=NULL){
        free_size = removed->size;
        free(removed);

        hole_t* new_hole = malloc(sizeof(hole_t));
        new_hole->position = position;
        new_hole->size = free_size;
        new_hole->prev = NULL;
        new_hole->next = NULL;

        holes_list_insert(&new_hole, &holes_head, &holes_tail);
    }
    pid_pos_map[pid] = UINT_MAX;
}

void best_fit_shutdown(){
    hole_t* current_hole = holes_head;
    used_position_t* current_used = used_head;
    hole_t* temp_h = NULL;
    used_position_t* temp_u = NULL;
    

    // Clean holes
    while(current_hole != NULL){
        temp_h= current_hole;
        current_hole = current_hole->next;
        free(temp_h);
    } 

    // Clean used positions 
    while(current_used != NULL){
        temp_u = current_used;
        current_used = current_used->next;
        free(temp_u);
    }
}
