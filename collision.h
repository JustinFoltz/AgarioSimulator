/**
 * @file collision.h
 * @brief header of module collision
 *
 * TODO main description
 *
 * @author Lucas Pallud, lucas.pallud@etu.hesge.ch
 * @author Justin Foltz, justin.foltz@etu.hesge.ch
 * @bug No known bugs.
 * @date June 02, 2019
 * @version 0.1
 */

#ifndef _COLLISION_
#define _COLLISION_

#include "cell.h"
#include "food.h"
#include "cell_queue.h"
#include "food_queue.h"
#include <pthread.h>

void apply_cell_collision(cell_t* eater, cell_t* eaten);
void apply_food_collision(bool** food_on_domain, cell_t* eater, food_t* eaten);
void apply_cell_collisions(queue_cell_t* queue);
void apply_food_collisions(queue_food_t* queue, bool** food_on_domain);

#endif