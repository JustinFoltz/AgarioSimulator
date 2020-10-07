/**
 * @file collision.c
 * @brief module to handle collision
 *
 * TODO main description
 *
 * @author Lucas Pallud, lucas.pallud@etu.hesge.ch
 * @author Justin Foltz, justin.foltz@etu.hesge.ch
 * @bug No known bugs.
 * @date June 02, 2019
 * @version 0.1
 */
#include "collision.h"
#include <stdbool.h>
#include <assert.h>

/**
 * Apply a collision on an eater cell and an eaten cell
 * 
 * @param eater the eater cell
 * @param eaten the eaten cell
 */
void apply_cell_collision(cell_t* eater, cell_t* eaten) {
    if (eater->alive && eaten->alive) {
        eater->mass += eaten->mass;
        eaten->alive = false;
    }
}

/**
 * Apply a collision on an eater cell and an eaten cell
 * 
 * @param food_on_domain an array to know where are foods on the game
 * @param eater the eater cell
 * @param eaten the eaten food
 */
void apply_food_collision(bool** food_on_domain, cell_t* eater, food_t* eaten) {
    assert(food_on_domain != NULL && "food_on_domain null");
    if (eater->alive && eaten->state == food_not_eaten) {
        eater->mass += 1;
        eaten->state = food_eaten;
        food_on_domain[eaten->y_position][eaten->x_position] = false;
    }
}

/**
 * Apply collisions on each node of the queue
 * 
 * @param queue the queue with the cells collisons
 */
void apply_cell_collisions(queue_cell_t* queue) {
    cell_t* eater;
    cell_t* eaten;
    while (cell_queue_dequeue(queue, &eater, &eaten) != -1) {
        apply_cell_collision(eater, eaten);
    }
}

/**
 * Apply collisions on each node of the queue
 * 
 * @param queue the queue with the cell food collisions
 * @param food_on_domain an array to know where are foods on the game
 */
void apply_food_collisions(queue_food_t* queue, bool** food_on_domain) {
    cell_t* eater;
    food_t* eaten;

    while (food_queue_dequeue(queue, &eater, &eaten) != -1) {
        apply_food_collision(food_on_domain, eater, eaten);
    }
}