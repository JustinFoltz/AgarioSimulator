/**
 * @file cell_queue.h
 * @brief header of module cell queue
 *
 * TODO main description
 *
 * @author Lucas Pallud, lucas.pallud@etu.hesge.ch
 * @author Justin Foltz, justin.foltz@etu.hesge.ch
 * @bug No known bugs.
 * @date June 02, 2019
 * @version 0.1
 */

#ifndef _CELL_QUEUE_
#define _CELL_QUEUE_

#include "cell.h"
#include <pthread.h>


typedef struct node_cell_t{
    cell_t* eater;
    cell_t* eaten;
    struct node_cell_t *next;
} node_cell_t;

typedef struct queue_cell_t{
    node_cell_t* head;
    node_cell_t* tail;
    pthread_mutex_t head_lock;
    pthread_mutex_t tail_lock;
} queue_cell_t;

queue_cell_t* cell_queue_init();
void cell_queue_enqueue(queue_cell_t* queue, cell_t* eater, cell_t* eaten);
int cell_queue_dequeue(queue_cell_t* queue, cell_t** eater, cell_t** eaten);
void cell_queue_destroy(queue_cell_t* queue);

#endif