/**
 * @file test_collision.c
 * @brief Unit test for the collision module
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
#include "collision.h"
#include "safe_thread.h"

void test_apply_cell_collision() {
    cell_t eater_alive; 
    eater_alive.alive = true;
    eater_alive.mass = 60;
    cell_t eaten_alive;
    eaten_alive.alive = true;
    eaten_alive.mass = 30;
    cell_t eater_dead;
    eater_dead.alive = false;
    eater_dead.mass = 100;
    cell_t eaten_dead;
    eaten_dead.alive = false;
    eaten_dead.mass = 20;

    apply_cell_collision(&eater_alive, &eaten_alive);
    assert(eater_alive.mass == 90);
    assert(eater_alive.alive == true);
    assert(eaten_alive.mass == 30);
    assert(eaten_alive.alive == false);
    apply_cell_collision(&eater_alive, &eaten_dead);
    assert(eater_alive.mass == 90);
    assert(eater_alive.alive == true);
    assert(eaten_dead.mass == 20);
    assert(eaten_dead.alive == false);
    eaten_alive.alive = true;
    apply_cell_collision(&eater_dead, &eaten_alive);
    assert(eater_dead.mass == 100);
    assert(eater_dead.alive == false);
    assert(eaten_alive.mass == 30);
    assert(eaten_alive.alive == true);
    apply_cell_collision(&eater_dead, &eaten_dead);
    assert(eater_dead.mass == 100);
    assert(eater_dead.alive == false);
    assert(eaten_dead.mass == 20);
    assert(eaten_dead.alive == false);

    printf("test_apply_collision : OK\n");
}

void test_apply_food_collision() {
    domain_t domain =  {25,25};
    bool** food_on_domain = initialize_food_on_domain(&domain);
    cell_t eater; 
    eater.alive = true;
    eater.mass = 60;
    eater.x = 10;
    eater.y = 10;
    food_t eaten;
    eaten.state = food_not_eaten;
    eaten.x_position = 10;
    eaten.y_position = 10;

    //eat not eaten food
    apply_food_collision(food_on_domain, &eater, &eaten); 
    assert(eater.mass == 61);
    assert(eaten.state == food_eaten);

    //eat food already eaten
    apply_food_collision(food_on_domain, &eater, &eaten); 
    assert(eater.mass == 61);
    assert(eaten.state == food_eaten);

    //dead cell eat food not eaten
    eater.alive = false;
    eaten.state = food_not_eaten;
    apply_food_collision(food_on_domain, &eater, &eaten);
    assert(eater.mass == 61);
    assert(eaten.state == food_not_eaten);

    //dead cell eat foot eaten
    eater.alive = false;
    eaten.state = food_eaten;
    apply_food_collision(food_on_domain, &eater, &eaten);
    assert(eater.mass == 61);
    assert(eaten.state == food_eaten);
    destroy_food_on_domain(food_on_domain, &domain);
    printf("test_apply_food_collision : OK\n");

}

/**
 * Program's entry point 
 * @return int program exit code
 */
int main() {
    printf("-----------------------\n");
    printf("test collisions : \n");
    printf("-----------------------\n");
    test_apply_cell_collision();
    test_apply_food_collision();
    return EXIT_SUCCESS;
}