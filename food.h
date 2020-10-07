/**
 * @file food.h
 * @brief food'module header
 *
 * TODO main description
 *
 * @author Lucas Pallud, lucas.pallud@etu.hesge.ch
 * @author Justin Foltz, justin.foltz@etu.hesge.ch
 * @bug No known bugs.
 * @date June 02, 2019
 * @version 0.1
 */

#ifndef _FOOD_
#define _FOOD_

#include "domain.h"
typedef enum food_state_t{
    food_not_eaten,
    food_eaten
} food_state_t;
typedef struct food_t{
    food_state_t state;
    int x_position;
    int y_position;
} food_t;

food_t* initialize_food(domain_t* domain, bool** food_on_domain, unsigned int* seed);
food_t** initialize_foods(domain_t* domain, bool** food_on_domain, int foods_number, unsigned int *seed);
bool **initialize_food_on_domain(domain_t* domain);
void maybe_reintroduce_food_in_game(domain_t* domain, bool** food_on_domain, food_t* food, unsigned int *seed, double probability);
void maybe_reintroduce_foods_in_game(domain_t* domain, bool** food_on_domain, food_t** foods, int foods_number, unsigned int *seed, double probability);
void define_new_food_position(domain_t* domain, bool** food_on_domain, food_t* food, unsigned int *seed);

void destroy_food_on_domain(bool** food_on_domain, domain_t* domain);
void free_foods(food_t** foods, int foods_number);

#endif