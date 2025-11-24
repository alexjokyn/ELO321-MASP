#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "first-fit.h"
#include "best-fit.h"
#include "best-fit-tdas.h"

// -------------------------------------------------------
// Metrics struct
// -------------------------------------------------------
typedef struct {
    unsigned int allocations;
    unsigned int failed_allocations;
    double success_ratio;
    double time_ms;
} metrics_t;

// -------------------------------------------------------
// FIRST-FIT WRAPPER
// -------------------------------------------------------
typedef struct {
    hole_t* holes_head;
    hole_t* holes_tail;

    used_position_t* used_head;
    used_position_t* used_tail;

    metrics_t metrics;
} firstfit_sim_t;


void firstfit_init(firstfit_sim_t* sim, unsigned int total_memory) {
    sim->holes_head = malloc(sizeof(hole_t));
    sim->holes_tail = sim->holes_head;
    sim->holes_head->position = 0;
    sim->holes_head->size = total_memory;
    sim->holes_head->prev = NULL;
    sim->holes_head->next = NULL;

    sim->used_head = NULL;
    sim->used_tail = NULL;

    sim->metrics.allocations = 0;
    sim->metrics.failed_allocations = 0;
}

void firstfit_alloc(firstfit_sim_t* sim, unsigned int pid, unsigned int size) {
    int ok = holes_list_request_first_fit(
        &sim->holes_head, &sim->holes_tail,
        &sim->used_head, &sim->used_tail,
        size, pid
    );

    if (ok == -1) {
        sim->metrics.failed_allocations++;
    } else {
        sim->metrics.allocations++;
    }
}

void firstfit_dealloc(firstfit_sim_t* sim, unsigned int pid) {
    (void)sim;
    (void)pid;
}

// -------------------------------------------------------
// TRACE PARSER
// -------------------------------------------------------
void simulate_first_fit(const char* filename, unsigned int mem_size, metrics_t* out) {
    firstfit_sim_t sim;
    firstfit_init(&sim, mem_size);

    clock_t start = clock();

    FILE* f = fopen(filename, "r");
    if (!f) { printf("ERROR: cannot open trace\n"); exit(1); }

    char op;
    unsigned int pid, size;

    unsigned int total_requests = 0;

    while (!feof(f)) {
        if (fscanf(f, " %c", &op) != 1) break;

        if (op == 'A') {
            fscanf(f, "%u %u", &pid, &size);
            firstfit_alloc(&sim, pid, size);
            total_requests++;
        } else if (op == 'F') {
            fscanf(f, "%u", &pid);
            firstfit_dealloc(&sim, pid);
        }
    }

    fclose(f);

    clock_t end = clock();
    double time_ms = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

    out->allocations        = sim.metrics.allocations;
    out->failed_allocations = sim.metrics.failed_allocations;
    out->success_ratio      = (total_requests == 0 ? 0.0 :
                               (double)sim.metrics.allocations / total_requests);
    out->time_ms            = time_ms;
}


// -------------------------------------------------------
// BEST-FIT SIMULATION
// -------------------------------------------------------
void simulate_best_fit(const char* filename, unsigned int mem_size, metrics_t* out) {

    best_fit_init(mem_size);

    metrics_t m = {0,0,0.0,0.0};

    clock_t start = clock();

    FILE* f = fopen(filename, "r");
    if (!f) { printf("ERROR: cannot open trace\n"); exit(1); }

    char op;
    unsigned int pid, size;

    unsigned int total_requests = 0;

    while (!feof(f)) {
        if (fscanf(f, " %c", &op) != 1) break;

        if (op == 'A') {
            fscanf(f, "%u %u", &pid, &size);

            int ok = best_fit_alloc(pid, size);
            if (ok == 1) m.allocations++;
            else m.failed_allocations++;

            total_requests++;

        } else if (op == 'F') {
            fscanf(f, "%u", &pid);
            best_fit_dealloc(pid);
        }
    }

    fclose(f);
    best_fit_shutdown();

    clock_t end = clock();
    double time_ms = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

    m.success_ratio = (total_requests == 0 ? 0.0 :
                      (double)m.allocations / total_requests);

    m.time_ms = time_ms;

    *out = m;
}


// -------------------------------------------------------
// MAIN
// -------------------------------------------------------
int main(int argc, char** argv) {

    if (argc < 3) {
        printf("Usage: ./sim trace.txt TOTAL_MEMORY\n");
        return 1;
    }

    const char* trace = argv[1];
    unsigned int mem_size = atoi(argv[2]);

    metrics_t first_m = {0};
    metrics_t best_m = {0};

    simulate_first_fit(trace, mem_size, &first_m);
    simulate_best_fit(trace, mem_size, &best_m);

printf("\n================ RESULTS ================\n");
printf("Total memory: %u\n", mem_size);

printf("\n------------ FIRST-FIT -----------------\n");
printf("Allocations OK:     %u\n", first_m.allocations);
printf("Allocations FAILED: %u\n", first_m.failed_allocations);
printf("Success ratio:      %.2f%%\n", first_m.success_ratio * 100);
printf("Time:               %.3f ms\n", first_m.time_ms);

printf("\n------------ BEST-FIT ------------------\n");
printf("Allocations OK:     %u\n", best_m.allocations);
printf("Allocations FAILED: %u\n", best_m.failed_allocations);
printf("Success ratio:      %.2f%%\n", best_m.success_ratio * 100);
printf("Time:               %.3f ms\n", best_m.time_ms);

printf("\n============ FINAL COMPARISON ==========\n");
printf("First-Fit success ratio: %.2f%%\n", first_m.success_ratio * 100);
printf("Best-Fit success ratio:  %.2f%%\n", best_m.success_ratio * 100);

printf("First-Fit faster?        %s\n",
       (first_m.time_ms < best_m.time_ms ? "YES" : "NO"));

printf("========================================\n");


    return 0;
}
