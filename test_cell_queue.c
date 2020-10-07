/**
 * @file test_collision_cell.c
 * @brief Unit test for the cell_queue module
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
#include "cell_queue.h"
#include "safe_thread.h"

void test_cell_queue_init_destroy() {
    queue_cell_t* queue = cell_queue_init();
    assert(queue->head != NULL);
    assert(queue->tail != NULL);
    cell_queue_destroy(queue);
    printf("test_cell_queue_init_destroy : OK\n");
}

void test_cell_queue_enqueue() {
    queue_cell_t* queue = cell_queue_init();
    cell_t eater;
    cell_t eaten;
    cell_queue_enqueue(queue, &eater, &eaten);
    assert(queue->head != NULL);
    assert(queue->tail->eater == &eater);
    assert(queue->tail->eaten == &eaten);

    cell_t eater2;
    cell_t eaten2;
    cell_queue_enqueue(queue, &eater2, &eaten2);
    assert(queue->tail->eater == &eater2);
    assert(queue->tail->eaten == &eaten2);
    assert(queue->head->next->eater == &eater);
    assert(queue->head->next->eaten == &eaten);
    printf("test_cell_queue_enqueue : OK\n");
    cell_queue_destroy(queue);
}

void test_cell_queue_dequeue() {
    queue_cell_t* queue = cell_queue_init();
    cell_t eater;
    cell_t eaten;
    cell_t eater2;
    cell_t eaten2;
    cell_queue_enqueue(queue, &eater, &eaten);
    cell_queue_enqueue(queue, &eater2, &eaten2);

    cell_t* eater_addr; 
    cell_t* eaten_addr;
    cell_t* eater2_addr;
    cell_t* eaten2_addr;

    int res = cell_queue_dequeue(queue, &eater_addr, &eaten_addr);
    assert(res == 0);
    assert(eater_addr == &eater);
    assert(eaten_addr == &eaten);

    res = cell_queue_dequeue(queue, &eater2_addr, &eaten2_addr);
    assert(res == 0);
    assert(eater2_addr == &eater2);
    assert(eaten2_addr == &eaten2);

    res = cell_queue_dequeue(queue, &eater2_addr, &eaten2_addr);
    assert(res == -1);

    printf("test_cell_queue_dequeue : OK\n");
    cell_queue_destroy(queue);
}
/**
 * Program's entry point 
 * @return int program exit code
 */
int main() {
    printf("-----------------------\n");
    printf("test cell_queue : \n");
    printf("-----------------------\n");
    test_cell_queue_init_destroy();
    test_cell_queue_enqueue();
    test_cell_queue_dequeue();
    return EXIT_SUCCESS;
}