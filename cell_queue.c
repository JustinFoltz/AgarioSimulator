/**
 * @file cell_queue.c
 * @brief module to handle cell queue
 *
 * TODO main description
 *
 * @author Lucas Pallud, lucas.pallud@etu.hesge.ch
 * @author Justin Foltz, justin.foltz@etu.hesge.ch
 * @bug No known bugs.
 * @date June 02, 2019
 * @version 0.1
 */

#include "cell_queue.h"
#include <stdlib.h>
#include "safe_thread.h"

/**
 * init a queue of cell collisions
 * 
 * @return queue_cell_t* 
 */
queue_cell_t* cell_queue_init() {
    queue_cell_t* queue = safe_malloc(sizeof(queue_cell_t));
    node_cell_t* tmp = safe_malloc(sizeof(node_cell_t));
    tmp->next = NULL;
    queue->head = queue->tail = tmp;
    pthread_mutex_init(&queue->head_lock, NULL);
    pthread_mutex_init(&queue->tail_lock, NULL);
    return queue;
}

/**
 * Enqueue an eater cell and an eaten cell in the queue
 * 
 * @param queue the queue in which we want to enqueue eater and eaten
 * @param eater the eater cell we want to enqueue
 * @param eaten the eaten cell we want to enqueue
 */
void cell_queue_enqueue(queue_cell_t* queue, cell_t* eater, cell_t* eaten) {
    node_cell_t* tmp = safe_malloc(sizeof(node_cell_t));
    tmp->eater = eater;
    tmp->eaten = eaten;
    tmp->next = NULL;
    safe_pthread_mutex_lock(&queue->tail_lock);
    queue->tail->next = tmp;
    queue->tail = tmp;
    safe_pthread_mutex_unlock(&queue->tail_lock);
}

/**
 * Dequeue an eater and an eaten cell
 * 
 * @param queue the queue in which we want to dequeue an eater and an eaten cells
 * @param eater a pointer to the eater cell we dequeue
 * @param eaten a pointer to the eaten cell we dequeue
 * @return int -1 if we try to dequeue an empty queue or 0 if the dequeue was successfull
 */
int cell_queue_dequeue(queue_cell_t* queue, cell_t** eater, cell_t** eaten) {
    safe_pthread_mutex_lock(&queue->head_lock);
    node_cell_t* tmp = queue->head;
    node_cell_t* new_head = tmp->next;
    if (new_head == NULL) {
        safe_pthread_mutex_unlock(&queue->head_lock);
        return -1;
    }
    *eater = new_head->eater;
    *eaten = new_head->eaten;
    queue->head = new_head;
    safe_pthread_mutex_unlock(&queue->head_lock);
    free(tmp);
    return 0;
}

/**
 * Remove all nodes of the queue and destroy it
 * 
 * @param queue the queue we want to clean and destroy
 */
void cell_queue_destroy(queue_cell_t* queue) {
    //first dequeue all the queue
    cell_t eater;
    cell_t* eater_addr = &eater;
    cell_t eaten;
    cell_t* eaten_addr = &eaten;
    int result = 0;
    do {
        result =  cell_queue_dequeue(queue, &eater_addr, &eaten_addr);
    } while (result != -1);

    safe_pthread_mutex_destroy(&queue->head_lock);
    safe_pthread_mutex_destroy(&queue->tail_lock);

    free(queue->head); // free the tmp node
    free(queue); 
}