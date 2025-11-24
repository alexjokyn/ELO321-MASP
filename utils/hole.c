# include "./hole.h"

void insert(hole* new_hole, hole** head, hole** tail) {

    // Empty list
    if (*head == NULL) {
        *head = new_hole;
        *tail = new_hole;
        new_hole->next = NULL;
        new_hole->prev = NULL;
        return;
    }

    hole* prev_to_new = NULL;
    hole* next_to_new = NULL;

    if ((prev_to_new = find_closest(*head, *tail, new_hole->position)) == NULL) {
        // Insert at begin
        new_hole->prev = NULL;
        new_hole->next = *head;
        (*head)->prev = new_hole;
        *head = new_hole;
    } else {
        // Insert after prev
        next_to_new = prev_to_new->next;
        new_hole->prev = prev_to_new;
        new_hole->next = prev_to_new->next;
        prev_to_new->next = new_hole;

        if (next_to_new != NULL) {next_to_new->prev = new_hole;}
        else {*tail = new_hole;}
    }

    // Apply coalescing (fusion) if necessary
    // Coalescing left
    if (new_hole->prev != NULL) {
        hole* left_neighbor = new_hole->prev;

        if (left_neighbor->position + left_neighbor->size == new_hole->position) {
            // Coalesce - the left neighbor will absorb the new hole
            left_neighbor->size += new_hole->size;
            // Remove new_hole
            left_neighbor->next = new_hole->next;

            if (new_hole->next != NULL) {new_hole->next->prev = left_neighbor;}
            else {*tail = left_neighbor;} // If now left_neighbor is the last at the list

            free(new_hole);
            new_hole = left_neighbor;
        }
    }

    // Coalescing right
    if (new_hole->next != NULL) {
        hole* right_neighbor = new_hole->next;

        if (new_hole->position + new_hole->size == right_neighbor->position) {
            // Absorb neighbor
            new_hole->size += right_neighbor->size;
            // Remove neighbor
            new_hole->next = right_neighbor->next;

            if (right_neighbor->next != NULL) {right_neighbor->next->prev = new_hole;}
            else {*tail = new_hole;} // If right_neighbor was the last one

            free(right_neighbor);
        }
    }
}



hole* find_best_fit(hole* head, hole* tail, unsigned int requested_size) {

    hole* best = NULL;
    hole* current = head;

    while (current != NULL) {

        // Not large enough
        if (current->size < requested_size) {
            current = current->next;
            continue;
        }

        // Exact fit
        if (current->size == requested_size) {
            return current;
        }

        // First valid fit OR better than previous best
        if (best == NULL || current->size < best->size) {
            best = current;
        }

        current = current->next;
    }

    return best;
}



hole* find_closest(hole* head, hole *tail, unsigned int position) {

    hole* current = head;

    while(current != NULL){
        if(current->position > position){
            return current->prev;
        }
        current = current->next;
    }

    return tail; //NULL if list is empty
}
