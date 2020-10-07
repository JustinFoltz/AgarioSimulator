/**
 * @file food_queue.c
 * @brief module to handle food_queue
 *
 * TODO main description
 *
 * @author Lucas Pallud, lucas.pallud@etu.hesge.ch
 * @author Justin Foltz, justin.foltz@etu.hesge.ch
 * @bug No known bugs.
 * @date June 02, 2019
 * @version 0.1
 */

#include "food_queue.h"
#include <stdlib.h>
#include "safe_thread.h"

/**
 * Init a queue of food
 * 
 * @return queue_food_t* 
 */
queue_food_t* food_queue_init() {
    queue_food_t* queue = safe_malloc(sizeof(queue_food_t));
    node_food_t* tmp = safe_malloc(sizeof(node_food_t));
    tmp->next = NULL;
    queue->head = queue->tail = tmp;
    safe_pthread_mutex_init(&queue->head_lock);
    safe_pthread_mutex_init(&queue->tail_lock);
    return queue;
}

/**
 * enqueue an eater cell and an eaten food in the queue
 * 
 * @param queue the queue
 * @param the eater cell we want to enqueue
 * @param the eaten food we want to enqueue
 */
void food_queue_enqueue(queue_food_t* queue, cell_t* eater, food_t* eaten) {
    node_food_t* tmp = safe_malloc(sizeof(node_food_t));

    tmp->eater = eater;
    tmp->eaten = eaten;
    tmp->next = NULL;

    safe_pthread_mutex_lock(&queue->tail_lock);
    queue->tail->next = tmp;
    queue->tail = tmp;
    safe_pthread_mutex_unlock(&queue->tail_lock);
}

/**
 * dequeue 
 * 
 * @param queue 
 * @param eater a pointer to the eater cell we dequeue
 * @param eaten a pointer to the eaten food we dequeue
 * @return int -1 if we try to dequeue and the queue is empty or 0 if the dequeue was successfull
 */
int food_queue_dequeue(queue_food_t* queue, cell_t** eater, food_t** eaten) {
    safe_pthread_mutex_lock(&queue->head_lock);
    node_food_t* tmp = queue->head;
    node_food_t* new_head = tmp->next;
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
 * remove all the remaining node in the queue and free the queue itself
 * 
 * @param queue the queue we want to clear and free
 */
void food_queue_destroy(queue_food_t* queue) {
    //first dequeue all the queue
    cell_t cell;
    cell_t* cell_addr = &cell;
    food_t food;
    food_t* food_addr = &food;
    int result = 0;
    do {
        result =  food_queue_dequeue(queue, &cell_addr, &food_addr);
    } while (result != -1);

    safe_pthread_mutex_destroy(&queue->head_lock);
    safe_pthread_mutex_destroy(&queue->tail_lock);

    free(queue->head); // free the tmp node
    free(queue); 
}