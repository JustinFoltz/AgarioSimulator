/**
 * @file cell.h
 * @brief cell's module header
 *
 * TODO main description
 *
 * @author Lucas Pallud, lucas.pallud@etu.hesge.ch
 * @author Justin Foltz, justin.foltz@etu.hesge.ch
 * @bug No known bugs.
 * @date June 02, 2019
 * @version 0.1
 */

#ifndef _CELL_
#define _CELL_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "domain.h"

#define ALPHA 1
#define BETA 18
#define GAMMA 0.2
#define DELTA 25
#define NB_COLORS 5

typedef struct {
    unsigned int red;
    unsigned int green;
    unsigned int blue;
} rgb_t;

typedef struct {
    int ux;
    int uy;
} vector_t;

typedef struct {
    rgb_t color;
    bool alive;
    vector_t direction;
    int x;
    int y;
    int mass;
} cell_t;

cell_t *initialize_cell(domain_t *domain, unsigned int *seed);
cell_t **initialize_cells(int nb_cells, domain_t *domain, unsigned int *seed);
void maybe_reintroduce_cell_in_game(domain_t* domain, cell_t* cell, unsigned int *seed, double probability);
void maybe_reintroduce_cells_in_game(domain_t* domain, cell_t** cells, int cells_number, unsigned int *seed, double probability);
void calculate_new_cell_position(cell_t *c, domain_t *domain, double p_change_direction, unsigned int *seed);
void assign_new_direction(cell_t *c, unsigned int *seed);
void handle_wall_colisions(cell_t *c, domain_t *domain);
void handle_direction_change(cell_t *c, double p_change_direction, unsigned int *seed);
int calculate_velocity(cell_t *c);
double calculate_radius(cell_t *c);
void free_cells(cell_t **cells, int nb_cells);
void assign_color(cell_t *cell, unsigned int *seed);
#endif