#ifndef FIRST_FIT_H
#define FIRST_FIT_H

#include "best-fit-tdas.h"


int holes_list_request_first_fit(
    hole_t** head_holes_list, hole_t** tail_holes_list,
    used_position_t** head_used_list, used_position_t** tail_used_list,
    unsigned int requested_size,
    unsigned int pid
);

#endif