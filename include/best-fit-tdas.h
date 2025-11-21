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

void holes_list_request_hole(hole_t** head, hole_t** tail, unsigned int requested_size);

// Double linked ordered list - used positions


// Queue - For the pending assigns of memory
