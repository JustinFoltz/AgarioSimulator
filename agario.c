/**
 * @file agario.c
 * @brief module to handle agario game
 *
 * TODO main description
 *
 * @author Lucas Pallud, lucas.pallud@etu.hesge.ch
 * @author Justin Foltz, justin.foltz@etu.hesge.ch
 * @bug No known bugs.
 * @date June 02, 2019
 * @version 0.1
 */

#include "agario.h"
#include "collision.h"


/**
 * @brief : detects if a collision occurs between c1 and c2 (unidirectional function)
 * @param c1 : eater cell
 * @param c2 : eaten cell
 * @return true 
 * @return false 
 */
bool detect_collision(cell_t *c1, cell_t *c2) {
    double radius = calculate_radius(c1);
    double distance = calculate_distance_beetwen_points(c1->x, c1->y, c2->x, c2->y);
    return distance < radius;
}

/**
 * @brief : detects if a collision occurs between a cell and a food
 * @param cell : eater cell
 * @param food : eaten food
 * @return true 
 * @return false 
 */
bool detect_food_collision(cell_t *cell, food_t *food) {
    double radius = calculate_radius(cell);
    double distance = calculate_distance_beetwen_points(cell->x, cell->y, food->x_position, food->y_position);
    return distance < radius;
}

/**
 * @brief : calculate the euclidien distance between two points from coordinates (2D)
 * @param x1 
 * @param y1 
 * @param x2 
 * @param y2 
 * @return double 
 */
double calculate_distance_beetwen_points(int x1, int y1, int x2, int y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) *  (y1 - y2));
}

/**
 * @brief : deplacement of cells by threads worker. 
 * @param w : structure parameters of threads worker
 */
void deplace(worker_t *w) {
    for(int i=w->cells_worker->start_index;i<w->cells_worker->stop_index; i++) {
        if(w->cells_worker->cells[i]->alive) {
            calculate_new_cell_position(w->cells_worker->cells[i], w->domain, w->cells_worker->p_dir, &w->cells_worker->seed);
        }
    }
}

/**
 * @brief : function call by threads worker.
 * @param args : structure parameters of threads worker
 * @return void* 
 */
void *works(void *args) {
    worker_t *w = (worker_t *)args;
    while(1) {
        deplace(w);
        safe_pthread_barrier_wait(w->b_deplace);
        detect_collisions(w);
        safe_pthread_barrier_wait(w->b_collision);
        if(*w->is_end) { return NULL; }
        safe_pthread_barrier_wait(w->b_collision);
    }
}

/**
 * @brief : function call by thread escape
 * @param args structure parameters of threads escape
 * @return void* 
 */
void *escape(void *args) {
    bool *is_escape = (bool *)args;
    while(1) {
        if( gfx_keypressed() == SDLK_ESCAPE ) { 
            *is_escape = true; 
            return NULL;
        }
        usleep(1.0/DISPLAY_FREQ*1e6);
    }
}

/**
 * @brief : function call by thread collider
 * @param args structure parameters of threads escape
 * @return void* 
 */
void *collide(void *args) {
    collisioner_t *collisioner = (collisioner_t *)args;
    struct timespec start_time, finish_time;

    struct gfx_context_t *ctxt = gfx_create("Agario", collisioner->domain->height, collisioner->domain->width);
     if (!ctxt) {
		fprintf(stderr, "Graphics initialization failed!\n");
		return NULL;
	}

    while(1) {

        clock_gettime(CLOCK_MONOTONIC, &start_time);

        if(*collisioner->is_escape) {*collisioner->is_end = true;}
        safe_pthread_barrier_wait(collisioner->b_collision);
        if(*collisioner->is_end) { 
            break;
        }
        queues_collisioner_t* queues = collisioner->queues_collisioner;     
        queue_cell_t* cell_queue = queues->queue_cells;
        queue_food_t* food_queue = queues->queue_foods;
        apply_cell_collisions(cell_queue);
        apply_food_collisions(food_queue, collisioner->food_on_domain);

        maybe_reintroduce_foods_in_game(collisioner->display_collisioner->domain,collisioner->food_on_domain, collisioner->display_collisioner->foods, collisioner->display_collisioner->nb_foods, collisioner->display_collisioner->seed, collisioner->display_collisioner->p_food);
        maybe_reintroduce_cells_in_game(collisioner->display_collisioner->domain, collisioner->display_collisioner->cells, collisioner->display_collisioner->nb_cells, collisioner->display_collisioner->seed, collisioner->display_collisioner->p_cell);
        safe_pthread_barrier_wait(collisioner->b_collision);
        clock_gettime(CLOCK_MONOTONIC, &finish_time);
        wait_for_display(start_time, finish_time, collisioner->display_collisioner->wait_time);

        render(ctxt, collisioner->display_collisioner->cells, 
            collisioner->display_collisioner->nb_cells, collisioner->display_collisioner->foods, collisioner->display_collisioner->nb_foods);
        gfx_present(ctxt);
    }

    gfx_destroy(ctxt);
    return NULL; 
}

/**
 * @brief : wait a specific time depending of execution time and target time for display
 * @param start_time : time of execution begining 
 * @param finish_time :time of execution ending
 * @param wait_time : target time
 */
void wait_for_display(struct timespec start_time, struct timespec finish_time, double wait_time) {
        double elapsed = (finish_time.tv_sec - start_time.tv_sec) * 1e6;
        elapsed += (finish_time.tv_nsec - start_time.tv_nsec) / 1e3;
        double time_to_wait = wait_time - elapsed;
        usleep(time_to_wait);
}

/**
 * @brief : detect collisions between cells by workers
 * @param w : structure parameters of threads worker
 */
void detect_collisions(worker_t *w) {
    for(int i=w->cells_worker->start_index;i<w->cells_worker->stop_index; i++) {
        bool is_collision = false;
        if(!w->cells_worker->cells[i]->alive) {continue;}
        for(int j=0; j<w->cells_worker->nb_cells; j++) {
            if(!w->cells_worker->cells[j]->alive) {continue;}
            if(i==j) {continue;}
            if(detect_collision(w->cells_worker->cells[i], w->cells_worker->cells[j])) {
                is_collision = true;
                if(is_bigger(w->cells_worker->cells[i], w->cells_worker->cells[j])) {
                    cell_queue_enqueue(w->cells_worker->queue_cells, w->cells_worker->cells[i], w->cells_worker->cells[j]);
                    break;
                }               
            }
        }
        if(is_collision) { continue; }
        for(int j=0; j<w->foods_worker->nb_foods; j++) {
            if(w->foods_worker->foods[j]->state) {continue;}
            if(detect_food_collision(w->cells_worker->cells[i], w->foods_worker->foods[j])) {
                food_queue_enqueue(w->foods_worker->queue_foods, w->cells_worker->cells[i], w->foods_worker->foods[j]);
                break;
            }
        }
    }
}

bool is_bigger(cell_t *c1, cell_t *c2) {
    return c1->mass > c2->mass;
}

/**
 * @brief : initialisation of a cell structure used by threads worker
 * @param cells : array of cells
 * @param nb_cells : nb rows of cells
 * @param start_index : start index of cell managed by a specific thread
 * @param stop_index : stop index of cell managed by a specific thread
 * @param p_dir : propability of a direction change occurs
 * @param seed : seed needed for reentrant random function
 * @param queue_cells : safe thread queue to stock cells collisions 
 * @return cells_thread_worker_t* 
 */
cells_thread_worker_t *initialize_cells_thread_worker(cell_t **cells, int nb_cells, int start_index, int stop_index, double p_dir, unsigned int *seed, queue_cell_t *queue_cells) {
    cells_thread_worker_t *cells_thread = safe_malloc(sizeof(cells_thread_worker_t));
    cells_thread->cells = cells;
    cells_thread->nb_cells = nb_cells;
    cells_thread->start_index = start_index;
    cells_thread->stop_index = stop_index;
    cells_thread->p_dir = p_dir;
    cells_thread->seed = rand_r(seed);
    cells_thread->queue_cells = queue_cells;
    return cells_thread;
}

/**
 * @brief : initialisation of a food structure used by threads worker
 * @param foods : array of food
 * @param nb_foods :nb rows of foods
 * @param seed : seed needed for reentrant random function
 * @param queue_foods : safe thread queue to stock cell/food collisions
 * @return foods_thread_worker_t* 
 */
foods_thread_worker_t *initialize_foods_thread_worker(food_t **foods, int nb_foods, unsigned int *seed, queue_food_t *queue_foods) {
    foods_thread_worker_t *foods_thread = safe_malloc(sizeof(foods_thread_worker_t));
    foods_thread->foods = foods;
    foods_thread->nb_foods = nb_foods;
    foods_thread->seed = rand_r(seed);
    foods_thread->queue_foods = queue_foods;
    return foods_thread;
}

/**
 * @brief : initialisation of an array of cell structures used by threads worker
 * @param nb_workers : number of threads worker
 * @param cells : array of cells
 * @param nb_cells : number rows of cells
 * @param p_dir : propability of a direction change occurs
 * @param seed : seed needed for reentrant random function
 * @param queue_cells : safe thread queue to stock cells collisions 
 * @return cells_thread_worker_t** 
 */
cells_thread_worker_t **initialize_cells_thread_workers(int nb_workers, cell_t **cells, int nb_cells, double p_dir, unsigned int *seed, queue_cell_t *queue_cells) {
    cells_thread_worker_t **cells_thread = safe_malloc(nb_workers*sizeof(cells_thread_worker_t*));
    int nb_cells_per_worker = nb_cells / nb_workers;
    int nb_remain_cells = nb_cells % nb_workers;
    int start, stop = 0;
    for(int i=0; i<nb_workers; i++) {
        start = stop;
        stop = start+ nb_cells_per_worker;
        if(nb_remain_cells > 0) {
            stop += 1;
            nb_remain_cells -= 1;
        }
        cells_thread[i] = initialize_cells_thread_worker(cells, nb_cells, start, stop, p_dir, seed, queue_cells);
    }
    return cells_thread;
}

/**
 * @brief : initialisation of an array of food structures used by threads worker
 * @param nb_workers : number of threads worker 
 * @param foods : array of foods
 * @param nb_foods : number of row of foods
 * @param seed : seed needed for reentrant random function
 * @param queue_foods : safe thread queue to stock cell/food collisions 
 * @return foods_thread_worker_t** 
 */
foods_thread_worker_t **initialize_foods_thread_workers(int nb_workers, food_t **foods, int nb_foods, unsigned int *seed, queue_food_t *queue_foods) {
    foods_thread_worker_t **foods_thread = safe_malloc(nb_workers*sizeof(foods_thread_worker_t*));
    for(int i=0; i<nb_workers; i++) {
        foods_thread[i] = initialize_foods_thread_worker(foods, nb_foods, seed, queue_foods);
    }
    return foods_thread;
}

/**
 * @brief : initialisation of a structure of worker
 * @param domain : structure defining limits of game display
 * @param is_end : define if game is over
 * @param cells_thread : cell structure for threads worker 
 * @param foods_thread : food structure for threads worker 
 * @param b_deplace : synchronisation barrier after deplace function
 * @param b_collision : synchronisation barrier after collide function
 * @return worker_t* 
 */
worker_t *initialise_worker(domain_t *domain, bool *is_end, cells_thread_worker_t *cells_thread, foods_thread_worker_t *foods_thread, pthread_barrier_t *b_deplace,
    pthread_barrier_t *b_collision) {
        worker_t *worker = safe_malloc(sizeof(worker_t));
        worker->domain = domain;
        worker->is_end = is_end;
        worker->cells_worker = cells_thread;
        worker->foods_worker = foods_thread;
        worker->b_deplace = b_deplace;
        worker->b_collision = b_collision;
        return worker;
}

/**
 * @brief : initialisation of an array of structures of worker
 * @param nb_workers : number of workers
 * @param domain : structure defining limits of game display
 * @param is_end : define is game is over
 * @param cells_thread : cell structure for threads worker 
 * @param foods_thread : food structure for threads worker 
 * @param b_deplace : synchronisation barrier after deplace function
 * @param b_collision : synchronisation barrier after collide function
 * @return worker_t** 
 */
worker_t **initialize_workers(int nb_workers, domain_t *domain, bool *is_end, cells_thread_worker_t **cells_thread, foods_thread_worker_t **foods_thread, pthread_barrier_t *b_deplace,
    pthread_barrier_t *b_collision) {
        worker_t **workers = safe_malloc(nb_workers*sizeof(worker_t*));
        for(int i=0; i<nb_workers; i++) {
            workers[i] = initialise_worker(domain, is_end, cells_thread[i], foods_thread[i], b_deplace, b_collision);
        }
        return workers;
}

/**
 * @brief : initialization of a safe queue structure for thread colider
 * @param q_cells : safe queue of cells collisions
 * @param q_foods : safe queue of foods collisions
 * @return queues_collisioner_t* 
 */
queues_collisioner_t *initialize_queues_collisioner(queue_cell_t *q_cells, queue_food_t *q_foods ) {
    queues_collisioner_t *queues_collisioner = safe_malloc(sizeof(queues_collisioner_t));
    queues_collisioner->queue_cells = q_cells;
    queues_collisioner->queue_foods = q_foods;
    return queues_collisioner;
}

/**
 * @brief : initialisation of a display structure for thread colider
 * @param domain : structure defining limits of game display
 * @param cells : array of cells
 * @param nb_cells : nb rows of cells
 * @param p_cells : probability of a cell to respawn
 * @param foods : array of foods
 * @param nb_foods : number of row of foods
 * @param p_foods : probability of a food to respawn
 * @param freq : display refresh frequency
 * @param seed : seed needed for reentrant random function
 * @return display_collisioner_t : 
 */
display_collisioner_t *initialize_display_collisioner(domain_t *domain, cell_t **cells, int nb_cells, double p_cells, food_t **foods, int nb_foods, double p_foods, int freq, unsigned int *seed) {
    display_collisioner_t *display_collisioner = safe_malloc(sizeof(display_collisioner_t));
    display_collisioner->domain = domain;
    display_collisioner->cells = cells;
    display_collisioner->nb_cells = nb_cells;
    display_collisioner->p_cell = p_cells;
    display_collisioner->foods = foods;
    display_collisioner->nb_foods = nb_foods;
    display_collisioner->p_food = p_foods;
    display_collisioner->wait_time = (1.0 / freq) * 1e6;
    display_collisioner->seed = seed;
    return display_collisioner;
}

/**
 * @brief : initialisation of a structure of colider
 * @param domain : structure defining limits of game display 
 * @param food_on_domain : boolean array of domain dimension defining living foods position
 * @param queues_collisioner : safe queue structure fro thread colider
 * @param display_collisioner : display structure for colider
 * @param b_collision : synchronisation barrier after collision function
 * @param is_escape : define if escape key is pressed
 * @param is_end : define if game is over
 * @return collisioner_t*
 */
collisioner_t *initialise_collisioner(domain_t *domain, bool** food_on_domain, queues_collisioner_t *queues_collisioner, display_collisioner_t *display_collisioner, pthread_barrier_t *b_collision, bool *is_escape, bool *is_end) {
    collisioner_t *collisioner = safe_malloc(sizeof(collisioner_t));
    collisioner->queues_collisioner = queues_collisioner;
    collisioner->display_collisioner = display_collisioner;
    collisioner->b_collision = b_collision;
    collisioner->is_end = is_end;
    collisioner->is_escape = is_escape;
    collisioner->domain = domain;
    collisioner->food_on_domain = food_on_domain;
    return collisioner;
}

/**
 * @brief : free memory allocated for workers
 * @param workers : worker structures
 * @param nb_workers : number of worker structures to free
 */
void free_workers(worker_t **workers, int nb_workers) {
    for( int i=0; i<nb_workers;i++) {
        free(workers[i]);
    }
    free(workers);
}

/**
 * @brief : free memory allocated for cells structures for workers
 * @param cells_worker : cells structures for workers
 * @param nb_workers : number of cells structures
 */
void free_cells_thread_workers(cells_thread_worker_t **cells_worker, int nb_workers) {
    for( int i=0; i<nb_workers;i++) {
            free(cells_worker[i]);
    }
    free(cells_worker);
}

/**
 * @brief : free memory allocated for foods structures for workers
 * @param foods_worker : foods structures for workers
 * @param nb_workers : number of foods structures
 */
void free_foods_thread_workers(foods_thread_worker_t **foods_worker, int nb_workers) {
    for( int i=0; i<nb_workers;i++) {
            free(foods_worker[i]);
    }
    free(foods_worker);
}






