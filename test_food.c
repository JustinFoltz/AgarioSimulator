/**
 * @file test_food.c
 * @brief Unit test for the food module
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
#include <stdbool.h>
#include "domain.h"
#include "food.h"
#include "safe_thread.h"

void test_initialize_food() {
    domain_t* domain = initialize_domain(10, 10);
    bool** food_on_domain = initialize_food_on_domain(domain);
    unsigned int seed = 1;
    food_t* food = initialize_food(domain, food_on_domain, &seed);
    assert(food->state == food_not_eaten);
    assert(food->x_position == 7);
    assert(food->y_position == 5);
    destroy_food_on_domain(food_on_domain, domain);
    free(domain);
    free(food);

    printf("test_initialize_food : OK\n");
}

void test_initialize_foods() {
    domain_t* domain = initialize_domain(10, 10);
    bool** food_on_domain = initialize_food_on_domain(domain);
    int foods_number = 5;
    unsigned int seed = 1;
    food_t** foods = initialize_foods(domain, food_on_domain, foods_number, &seed);

    assert(foods[0]->state == food_not_eaten);
    assert(foods[0]->x_position == 7);
    assert(foods[0]->y_position == 5);
    assert(foods[1]->state == food_not_eaten);
    assert(foods[1]->x_position == 2);
    assert(foods[1]->y_position == 3);
    assert(foods[2]->state == food_not_eaten);
    assert(foods[2]->x_position == 1);
    assert(foods[2]->y_position == 3);
    assert(foods[3]->state == food_not_eaten);
    assert(foods[3]->x_position == 4);
    assert(foods[3]->y_position == 6);
    assert(foods[4]->state == food_not_eaten);
    assert(foods[4]->x_position == 3);
    assert(foods[4]->y_position == 4);

    free_foods(foods, foods_number);
    destroy_food_on_domain(food_on_domain, domain);
    free(domain);

    printf("test_initialize_foods : OK\n");
}
void test_mayber_reintroduce_food_in_game() {
    domain_t *domain = initialize_domain(10, 10);
    bool** food_on_domain = initialize_food_on_domain(domain);
    food_t food_1 = {food_eaten, 1,1};
    unsigned int seed = 1;
    maybe_reintroduce_food_in_game(domain, food_on_domain, &food_1, &seed, 1.0);
    assert(food_1.state == food_not_eaten);
    assert(food_1.x_position == 5);
    assert(food_1.y_position == 2);

    food_t food_2 = {food_eaten, 2,2};
    maybe_reintroduce_food_in_game(domain, food_on_domain, &food_2, &seed, 0.0);
    assert(food_2.state == food_eaten);
    assert(food_2.x_position == 2);
    assert(food_2.y_position == 2);

    destroy_food_on_domain(food_on_domain, domain);
    free(domain);

    printf("test_maybe_reintroduce_food_in_game : OK\n");
}

/**
 * Program's entry point 
 * 
 * @param argc number of command line arguments
 * @param argv array of strings storing the command line arguments
 * @return int program exit code
 */
int main() {
    printf("-----------------------\n");
    printf("test food : \n");
    printf("-----------------------\n");
    test_initialize_foods();
    test_initialize_food();
    test_mayber_reintroduce_food_in_game();
    return EXIT_SUCCESS;
}