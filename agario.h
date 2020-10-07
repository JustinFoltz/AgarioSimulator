/**
 * @file agario.h
 * @brief agario's module header
 *
 * TODO main description
 *
 * @author Lucas Pallud, lucas.pallud@etu.hesge.ch
 * @author Justin Foltz, justin.foltz@etu.hesge.ch
 * @bug No known bugs.
 * @date June 02, 2019
 * @version 0.1
 */

#ifndef _AGARIO_
#define _AGARIO_

#define DISPLAY_FREQ 50.0

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h> 
#include <unistd.h>
#include "cell.h"
#include "food.h"
#include "cell_queue.h"
#include "food_queue.h"
#include "safe_thread.h"
#include "./lib/gfx.h"
#include "collision.h"

/**
 * @brief Parameters related to cells 
 *  Used by workers threads
 * 
 */
typedef struct {
    cell_t **cells;
    int nb_cells;
    int start_index;
    int stop_index;
    double p_dir;
    unsigned int seed;
    queue_cell_t *queue_cells;
} cells_thread_worker_t;

/**
 * @brief Parameters related to food 
 *  Used by workers threads
 * 
 */
typedef struct {
    food_t **foods;
    int nb_foods;
    unsigned int seed;
    queue_food_t *queue_foods;
} foods_thread_worker_t;

/**
 * @brief Global parameters used by workers threads
 * 
 */
typedef struct {
    domain_t *domain;
    bool *is_end;
    cells_thread_worker_t *cells_worker;
    foods_thread_worker_t *foods_worker;
    pthread_barrier_t *b_deplace;
    pthread_barrier_t *b_collision;
} worker_t;

/**
 * @brief Parameters related to collisions
 *  Used by collisioner thread
 */
typedef struct {
    queue_cell_t *queue_cells;
    queue_food_t *queue_foods;
} queues_collisioner_t;

/**
 * @brief Parameters related to display
 *  Used by collisioner thread
 */
typedef struct {
    double wait_time;
    domain_t *domain;
    cell_t **cells;
    int nb_cells;
    food_t **foods;
    int nb_foods;
    int unsigned *seed;
    double p_food;
    double p_cell;
} display_collisioner_t;

/**
 * @brief Global parameters used by workers threads
 * 
 */
typedef struct {
    queues_collisioner_t *queues_collisioner;
    display_collisioner_t *display_collisioner;
    pthread_barrier_t *b_collision;
    bool *is_escape;
    bool *is_end;
    domain_t *domain;
    bool** food_on_domain;
} collisioner_t;

void *works(void *args);
void *escape(void *args);
void *collide(void *args);
void deplace(worker_t *w);
void detect_collisions(worker_t *w);

bool detect_collision(cell_t *c1, cell_t *c2);
double calculate_distance_beetwen_points(int x1, int y1, int x2, int y2);
void wait_for_display(struct timespec start_time, struct timespec finish_time, double wait_time);
bool is_bigger(cell_t *c1, cell_t *c2);

cells_thread_worker_t *initialize_cells_thread_worker(cell_t **cells, int nb_cells, int start_index, int stop_index, double p_dir, unsigned int *seed, queue_cell_t *queue_cells);
foods_thread_worker_t *initialize_foods_thread_worker(food_t **foods, int nb_foods, unsigned int *seed, queue_food_t *queue_foods);
cells_thread_worker_t **initialize_cells_thread_workers(int nb_workers, cell_t **cells, int nb_cells, double p_dir, unsigned int *seed, queue_cell_t *queue_cells);
foods_thread_worker_t **initialize_foods_thread_workers(int nb_workers, food_t **foods, int nb_foods, unsigned int *seed, queue_food_t *queue_foods);
worker_t *initialise_worker(domain_t *domain, bool *is_end, cells_thread_worker_t *cells_thread, foods_thread_worker_t *foods_thread, pthread_barrier_t *b_deplace, pthread_barrier_t *b_collision);
worker_t **initialize_workers(int nb_workers, domain_t *domain, bool *is_end, cells_thread_worker_t **cells_thread, foods_thread_worker_t **foods_thread, pthread_barrier_t *b_deplace, pthread_barrier_t *b_collision);
queues_collisioner_t *initialize_queues_collisioner(queue_cell_t *q_cells, queue_food_t *q_foods );
display_collisioner_t *initialize_display_collisioner(domain_t *domain, cell_t **cells, int nb_cells, double p_cells, food_t **foods, int nb_foods, double p_foods, int freq, unsigned int *seed);
collisioner_t *initialise_collisioner(domain_t *domain, bool** food_on_domain, queues_collisioner_t *queues_collisioner, display_collisioner_t *display_collisioner, pthread_barrier_t *b_collision, bool *is_escape, bool *is_end);

void free_cells_thread_workers(cells_thread_worker_t **cells_worker, int nb_workers);
void free_foods_thread_workers(foods_thread_worker_t **foods_worker, int nb_workers);
void free_workers(worker_t **workers, int nb_workers);



#endif
