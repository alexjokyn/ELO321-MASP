#ifndef DEN_H
#define DEN_H


//------------------------------------------------
//------------- Struct Definition ----------------
//------------------------------------------------

// Doubly Linked List - dataType den
typedef struct den{
    unsigned int pid;
    unsigned int position;
    unsigned int size:
    struct den* prev;
    struct den* next;
} den;


//------------------------------------------------
//------------ Function Prototypes ---------------
//------------------------------------------------

// Inserts new den (allocates process)
void insert(den* new_den, den** head, den** tail);

// Removes den (deallocates process)
den* remove(den** head, den** tail, unsigned int position);

// Finds the closest previous den in memory (lower position)
den* closest(den* head, den *tail, unsigned int position);


#endif //DEN_H
