/**
 * @file collision_food.h
 * @brief header of module collision_food
 *
 * TODO main description
 *
 * @author Lucas Pallud, lucas.pallud@etu.hesge.ch
 * @author Justin Foltz, justin.foltz@etu.hesge.ch
 * @bug No known bugs.
 * @date June 02, 2019
 * @version 0.1
 */

#ifndef _FOOD_QUEUE_
#define _FOOD_QUEUE_

#include "cell.h"
#include "food.h"
#include <pthread.h>

typedef struct node_food_t{
    cell_t* eater;
    food_t* eaten;
    struct node_food_t *next;
} node_food_t;

typedef struct queue_food_t{
    node_food_t* head;
    node_food_t* tail;
    pthread_mutex_t head_lock;
    pthread_mutex_t tail_lock;
} queue_food_t;


queue_food_t* food_queue_init();
void food_queue_enqueue(queue_food_t* queue, cell_t* eater, food_t* eaten);
int food_queue_dequeue(queue_food_t* queue, cell_t** eater, food_t** eaten);
void food_queue_destroy(queue_food_t* queue);

#endif