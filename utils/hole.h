#ifndef HOLE_H
#define HOLE_H

#include <stdlib.h>

//------------------------------------------------
//------------- Struct Definition ----------------
//------------------------------------------------

// Doubly Linked List - dataType hole
typedef struct hole{
    unsigned int position;
    unsigned int size:
    struct hole* prev;
    struct hole* next;
} hole;


//------------------------------------------------
//------------ Function Prototypes ---------------
//------------------------------------------------

// Inserts new hole
void insert(hole* new_hole, hole** head, hole** tail);

// Finds the tiniest hole, big enough to contain the requested memory size
hole* find_best_fit(hole* head, hole* tail, unsigned int requested_size);

// Finds the closest previous hole in memory (lower position)
hole* find_closest(hole* head, hole *tail, unsigned int position);


#endif //HOLE_H
