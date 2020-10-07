/**
 * @file test_food_queue.c
 * @brief Unit test for the food_queue module
 *
 * TODO main description
 *
 * @author Lucas Pallud, lucas.pallud@etu.hesge.ch
 * @author Justin Foltz, justin.foltz@etu.hesge.ch
 * @bug No known bugs.
 * @date June 02, 2019
 * @version 0.1
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "food_queue.h"
#include "safe_thread.h"

void test_food_queue_init_destroy() {
    queue_food_t* queue = food_queue_init();
    assert(queue->head != NULL);
    assert(queue->tail != NULL);
    food_queue_destroy(queue);
    printf("test_food_queue_init_destroy : OK\n");
}

void test_food_queue_enqueue() {
    queue_food_t* queue = food_queue_init();
    cell_t eater;
    food_t eaten;
    food_queue_enqueue(queue, &eater, &eaten);
    assert(queue->head != NULL);
    assert(queue->tail->eater == &eater);
    assert(queue->tail->eaten == &eaten);


    cell_t eater2;
    food_t eaten2;
    food_queue_enqueue(queue, &eater2, &eaten2);
    assert(queue->tail->eater == &eater2);
    assert(queue->tail->eaten == &eaten2);
    assert(queue->head->next->eaten == &eaten);
    assert(queue->head->next->eater == &eater);
    printf("test_food_queue_enqueue : OK\n");
    food_queue_destroy(queue);
}

void test_food_queue_dequeue() {
    queue_food_t* queue = food_queue_init();
    cell_t eater;
    food_t eaten;
    cell_t eater2;
    food_t eaten2;
    food_queue_enqueue(queue, &eater, &eaten);
    food_queue_enqueue(queue, &eater2, &eaten2);

    cell_t* eater_addr = &eater;
    food_t* eaten_addr = &eaten;
    cell_t* eater2_addr = &eater2;
    food_t* eaten2_addr = &eaten2;

    int res = food_queue_dequeue(queue, &eater_addr, &eaten_addr);
    assert(res == 0);
    assert(eater_addr == &eater);
    assert(eaten_addr == &eaten);

    res = food_queue_dequeue(queue, &eater2_addr, &eaten2_addr);
    assert(res == 0);
    assert(eater2_addr == &eater2);
    assert(eaten2_addr == &eaten2);

    res = food_queue_dequeue(queue, &eater2_addr, &eaten2_addr);
    assert(res == -1);

    printf("test_food_queue_dequeue : OK\n");
    food_queue_destroy(queue);
}


/**
 * Program's entry point 
 * @return int program exit code
 */
int main() {
    printf("-----------------------\n");
    printf("test food_queue : \n");
    printf("-----------------------\n");

    test_food_queue_init_destroy();
    test_food_queue_enqueue();
    test_food_queue_dequeue();
    return EXIT_SUCCESS;
}