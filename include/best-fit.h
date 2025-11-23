#ifndef BEST_FIT_H
#define BEST_FIT_H


#define MAX_PID_MAP 4096

/* Initialize best fit allocator algorithm.
 * Returns:
 * 0 : fail
 * 1 : success
 */
int best_fit_init(unsigned int total_memory_size);

/* Allocate a piece of memory
 * of size = size for processs
 * with PID = pid
 * Returns:
 * -1 : PID is greater than simulator limit
 *  0 : Could not allocate memory
 *  1 : Success
 */
int best_fit_alloc(unsigned int pid, unsigned int size);

/* Deallocates the memory assigned to 
 * process with PID = pid.
 * It DOES CHECK for valid pid.
 */
void best_fit_dealloc(unsigned int pid);

/*
 * Cleans the best fit allocator algorithm
* and the memory used
 */
void best_fit_shutdown();

#endif
