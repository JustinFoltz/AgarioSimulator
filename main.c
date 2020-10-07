/**
 * @file main.c
 * @brief The main of the simulator 
 *
 * TODO main description
 *
 * @author Lucas Pallud, lucas.pallud@etu.hesge.ch
 * @author Justin Foltz, justin.foltz@etu.hesge.ch
 * @bug No known bugs.
 * @date May 19, 2019
 * @version 0.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "cell.h"
#include "agario.h"
#include "cell_queue.h"
#include "food_queue.h"
#include "safe_thread.h"


/**
 * Program's entry point 
 * 
 * @param argc number of command line arguments
 * @param argv array of strings storing the command line arguments
 * @return int program exit code
 */

int main(int argc, char** argv) {

    if(argc!=11) {
        printf("Args must be : <width> <height> <seed> <food> <dir> <res> <nf> <freq> <workers> <cells>\n");
        return EXIT_FAILURE;
    }

    /**
     * @brief Parsing of arguments
     * 
     */
    domain_t domain = {atoi(argv[1]), atoi(argv[2])};
    unsigned int seed = atoi(argv[3]);
    int total_surface = domain.height * domain.height;
    double surface_food;
    sscanf(argv[4], "%lf", &surface_food);
    int nb_foods = (int)(surface_food * total_surface);
    double p_dir, p_cell, p_food, freq;
    sscanf(argv[5], "%lf", &p_dir);
    sscanf(argv[6], "%lf", &p_cell);
    sscanf(argv[7], "%lf", &p_food);
    sscanf(argv[8], "%lf", &freq);
    int nb_workers = atoi(argv[9]);
    int nb_cells = atoi(argv[10]); 


    /**
     * @brief Initialization of parameters
     * 
     */
    bool is_end = false;
    bool is_escape = false;

    pthread_barrier_t b_deplace;
    safe_pthread_barrier_init(&b_deplace, nb_workers);

    pthread_barrier_t b_collision;
    safe_pthread_barrier_init(&b_collision, (nb_workers+1));

    bool** food_on_domain = initialize_food_on_domain(&domain);

    queue_cell_t *q_cells = cell_queue_init();
    queue_food_t *q_food = food_queue_init();

    cell_t **cells = initialize_cells(nb_cells, &domain, &seed);
    food_t **foods = initialize_foods(&domain, food_on_domain, nb_foods,  &seed);


    /**
     * @brief Initialization of threads
     * 
     */
    cells_thread_worker_t **cells_workers = initialize_cells_thread_workers(nb_workers, cells, nb_cells, p_dir, &seed, q_cells);
    foods_thread_worker_t **foods_workers = initialize_foods_thread_workers(nb_workers, foods, nb_foods, &seed, q_food);
    worker_t **workers = initialize_workers(nb_workers, &domain, &is_end, cells_workers, foods_workers, &b_deplace, &b_collision);
    pthread_t thread_workers[nb_workers];

    queues_collisioner_t *queues_collisioner = initialize_queues_collisioner(q_cells, q_food);
    display_collisioner_t *display_colllisioner = initialize_display_collisioner(&domain, cells, nb_cells, p_cell, foods, nb_foods, p_food, freq, &seed);
    collisioner_t *collisioner = initialise_collisioner(&domain,food_on_domain , queues_collisioner, display_colllisioner, &b_collision, &is_escape, &is_end);
    pthread_t thread_collisioner;

    pthread_t thread_escape;


    /**
     * @brief Launching of threads
     * 
     */
    for(int i=0; i<nb_workers; i++) {
        safe_pthread_create(&thread_workers[i], works, workers[i]);
    }
    safe_pthread_create(&thread_collisioner, collide, collisioner);
    safe_pthread_create(&thread_escape, escape, &is_escape);


    /**
     * @brief Capture of threads
     * 
     */
    for(int i=0; i<nb_workers;i++) {
        safe_pthread_join(thread_workers[i], NULL);
    }
    safe_pthread_join(thread_collisioner, NULL);
    safe_pthread_join(thread_escape, NULL);


    /**
     * @brief Free memory
     * 
     */
    safe_pthread_barrier_destroy(&b_deplace);
    safe_pthread_barrier_destroy(&b_collision);
    cell_queue_destroy(q_cells);
    food_queue_destroy(q_food);
    free_cells(cells, nb_cells);
    free_foods(foods, nb_foods);

    free_cells_thread_workers(cells_workers, nb_workers);
    free_foods_thread_workers(foods_workers, nb_workers);
    free_workers(workers, nb_workers);
    destroy_food_on_domain(food_on_domain, &domain);

    free(queues_collisioner);
    free(display_colllisioner);
    free(collisioner);

    return EXIT_SUCCESS;
}