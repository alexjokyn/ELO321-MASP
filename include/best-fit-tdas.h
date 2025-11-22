#include <stdlib.h>
#include <limits.h>

typedef struct hole_t{
    unsigned int position;
    unsigned int size;
    struct hole_t* prev;
    struct hole_t* next;
} hole_t;

typedef struct used_position_t{
    unsigned int pid;
    unsigned int position;
    unsigned int size;
    struct used_position_t* prev;
    struct used_position_t* next;
} used_position_t;


// Double linked ordered list - holes


void holes_list_insert(hole_t** hole, hole_t** head, hole_t** tail);

hole_t* holes_list_find_best_fit(hole_t* head, hole_t* tail, unsigned int requested_size);

hole_t* holes_list_find_closest_hole(hole_t* head, hole_t* tail, unsigned int position); // The hole previous to such position

// Executes the allocation and returns the position of the allocated space - returns -1 if failure
unsigned int holes_list_request_hole(hole_t** head, hole_t** tail, unsigned int requested_size, unsigned int pid, used_position_t** head_used_list, used_position_t** tail_used_list);

// Double linked ordered list - used positions

void used_list_insert(used_position_t* new_alloc, used_position_t** head, used_position_t** tail);

used_position_t* used_list_remove(used_position_t** head, used_position_t** tail, unsigned int position);

used_position_t* used_list_find_closest_position(used_position_t* head, used_position_t* tail, unsigned int position); // The used position previous to such position

// Queue - For the pending assigns of memory
