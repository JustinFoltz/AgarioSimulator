/**
 * @file food.c
 * @brief module to handle food
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
#include <stdbool.h>
#include <assert.h>
#include "food.h"
#include "domain.h"
#include "random.h"
#include "safe_thread.h"

/**
 * Reintroduce a food in the game according to a probability
 * 
 * @param domain the domain of the game
 * @param food_on_domain an array to know where foods are in the game
 * @param food the food we want to reintroduce
 * @param seed the seed for the random generator
 * @param probability the probability of reintroduction
 */
void maybe_reintroduce_food_in_game(domain_t* domain, bool** food_on_domain, food_t* food, unsigned int *seed, double probability) {
    double random_number = rand_r_min_max(seed, 0, 100) / 100.0;
    if (random_number <= probability) {
        food->state = food_not_eaten;
        define_new_food_position(domain, food_on_domain, food, seed);
    }
}
/**
 * Reintroduce dead foods according to a probability
 * 
 * @param domain the domain of the game
 * @param food_on_domain an array to know where foods are in the game
 * @param foods an array with all the foods
 * @param foods_number the foods number in the foods array
 * @param seed the seed for the random generator
 * @param probability the probaility of reintroduction for each food
 */
void maybe_reintroduce_foods_in_game(domain_t* domain, bool** food_on_domain, food_t** foods, int foods_number, unsigned int *seed, double probability) {
    for (int i = 0; i < foods_number; i++) {
        if (foods[i]->state == food_eaten) {
            maybe_reintroduce_food_in_game(domain, food_on_domain, foods[i], seed, probability);
        }
    }
}

/**
 * Allocate and initialize an array to know where there is already foods
 * 
 * @param domain the domain of the game
 * @return a 2 dimensions array initialized at false
 */
bool **initialize_food_on_domain(domain_t* domain) {
    bool** food_on_domain = safe_malloc(domain->width*sizeof(bool*));
    for (int i = 0; i < domain->width; i++) {
        food_on_domain[i] = safe_malloc(domain->height*sizeof(bool));
    }
    for (int i = 0; i < domain->width; i++) {
        for (int j = 0; j < domain->height; j++) {
            food_on_domain[i][j] = false ;
        }
    }
    return food_on_domain;
}

/**
 * free the food on domain array 
 * 
 * @param food_on_domain the food on domain array we want to free
 * @param domain the domain of the game
 */
void destroy_food_on_domain(bool** food_on_domain, domain_t* domain) {
    for (int i = 0; i < domain->width; i++) {
        free(food_on_domain[i]);
    }
    free(food_on_domain);
}

/**
 * define a new random position for a food
 * 
 * @param domain the domain of the game
 * @param food_on_domain an array to know where foods are in the game
 * @param food the food we want a new position for
 * @param seed the seed for the random generator
 */
void define_new_food_position(domain_t* domain, bool** food_on_domain, food_t* food, unsigned int *seed) {
    assert(food_on_domain != NULL && "food_on_domain null");
    int x, y;
    do {
        x = rand_r_min_max(seed, 1, domain->height-1);
        y = rand_r_min_max(seed, 1, domain->width-1);
    } while (food_on_domain[y][x]);

    food->x_position = x;
    food->y_position = y;
    food_on_domain[y][x] = true;
}

/**
 * allocate a food and add a random position to it.
 * 
 * @param domain the domain of the game
 * @param food_on_domain an array to know where foods are in the game
 * @param seed  the seed for the random generator
 * @return food_t* 
 */
food_t* initialize_food(domain_t* domain, bool** food_on_domain, unsigned int* seed) {
    food_t* a_food = safe_malloc(sizeof(food_t));
    a_food->state = food_not_eaten;
    define_new_food_position(domain, food_on_domain, a_food, seed);
    return a_food;
}

/**
 * Allocate a number of foods with random positions
 * 
 * @param domain the domain of the game
 * @param food_on_domain an array to know where foods are in the game
 * @param foods_number the number of foods we want
 * @param seed the seed for the random generator
 * @return food_t** an array of food
 */
food_t** initialize_foods(domain_t* domain, bool** food_on_domain, int foods_number, unsigned int *seed) {
    food_t** foods = safe_malloc(foods_number * sizeof(food_t*));
    for (int i = 0; i < foods_number; i++) {
        foods[i] = initialize_food(domain, food_on_domain, seed);
    }
    return foods;
}

/**
 * free all the foods
 * 
 * @param foods the array of foods we want to free
 * @param foods_number the number of food in the foods array
 */
void free_foods(food_t** foods, int foods_number) {
    for (int i = 0; i < foods_number; i++) {
        free(foods[i]);
    }
    free(foods);
}