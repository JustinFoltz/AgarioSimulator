/**
 * @file test_cell.c
 * @brief Unit test for the cell module
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
#include "cell.h"
#include "safe_thread.h"

void test_calculate_radius() {
    cell_t cell;

    cell.mass = 1;
    double radius_min = calculate_radius(&cell);

    cell.mass = 2;
    double radius_2 = calculate_radius(&cell);

    assert(radius_min == 10.0);
    assert(radius_min < radius_2);
    printf("test_calculate_radius : OK\n");
}

void test_calculate_velocity() {
    cell_t cell;
    cell.mass = 1;
    int velocity_max = calculate_velocity(&cell);

    cell.mass = 20;
    int velocity_middle = calculate_velocity(&cell);

    cell.mass = 100;
    int velocity_min = calculate_velocity(&cell);

    assert(velocity_max == BETA);
    assert(velocity_min == ALPHA);
    assert(velocity_middle > velocity_min && velocity_middle < velocity_max);
    printf("test_calculate_velocity : OK\n");
}

void text_calculate_new_cell_position() {
    domain_t domain = {50,50};
    unsigned int seed = 10;
    cell_t cell;
    cell.mass = 1;
    cell.x = 20;
    cell.y = 30;

    cell.direction.ux = -1;
    cell.direction.uy = 0;
    calculate_new_cell_position(&cell, &domain, 0, &seed);
    assert(cell.x == 2);
    assert(cell.y == 30);
    assert(cell.direction.ux==-1 && cell.direction.uy==0);

    cell.direction.ux = 1;
    cell.direction.uy = 1;
    calculate_new_cell_position(&cell, &domain, 0, &seed);
    assert(cell.x == 20);
    assert(cell.y == 48);
    assert(cell.direction.ux==1 && cell.direction.uy==1);

    cell.direction.ux = -1;
    cell.direction.uy = 1;
    calculate_new_cell_position(&cell, &domain, 0, &seed);
    assert(cell.x == 2);
    assert(cell.y == 34);
    assert(cell.direction.ux==-1 && cell.direction.uy==-1);

    cell.direction.ux = -1;
    cell.direction.uy = 1;
    calculate_new_cell_position(&cell, &domain, 0, &seed);
    assert(cell.x == 16);
    assert(cell.y == 48);
    assert(cell.direction.ux==1 && cell.direction.uy==-1);

    for(int i=0; i<5; i++) {
        int direction_x = cell.direction.ux;
        int direction_y = cell.direction.uy;
        calculate_new_cell_position(&cell, &domain, 1, &seed);
        calculate_new_cell_position(&cell, &domain, 1, &seed);
        assert(cell.direction.ux!=direction_x || cell.direction.uy!=direction_y);
    }

    printf("text_calculate_new_cell_position : OK\n");
}

void test_initialize_cells() {
    int nb_cells = 5;
    domain_t domain = {50,50};
    unsigned int seed = 5;

    cell_t **cells = initialize_cells(nb_cells, &domain, &seed);
    for( int i=0; i<nb_cells; i++) {
        assert(cells[i]->alive == true);
        assert(cells[i]->direction.ux >= -1 && cells[i]->direction.ux <= 1);
        assert(cells[i]->direction.uy >= -1 && cells[i]->direction.uy <= 1);
        assert(cells[i]->mass >= 5 && cells[i]->mass <= 8);
        assert(cells[i]->x > 0 && cells[i]->x < domain.height);
        assert(cells[i]->y > 0 && cells[i]->y < domain.width);
    }


    assert(cells[0]->mass==6 && cells[0]->x==6 && cells[0]->y==20);
    assert(cells[1]->mass==5 && cells[1]->x==49 && cells[1]->y==16);
    assert(cells[2]->mass==8 && cells[2]->x==14 && cells[2]->y==12);
    assert(cells[3]->mass==6 && cells[3]->x==19 && cells[3]->y==19);
    assert(cells[4]->mass==5 && cells[4]->x==41 && cells[4]->y==35);
    
    free_cells(cells, nb_cells);
    printf("initialize_cells : OK\n");
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
    printf("test cells : \n");
    printf("-----------------------\n");
    test_calculate_radius();
    test_calculate_velocity();
    text_calculate_new_cell_position();
    test_initialize_cells();
    return EXIT_SUCCESS;
}