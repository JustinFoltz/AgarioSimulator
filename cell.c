/**
 * @file cell.c
 * @brief module to handle cells
 *
 * TODO main description
 *
 * @author Lucas Pallud, lucas.pallud@etu.hesge.ch
 * @author Justin Foltz, justin.foltz@etu.hesge.ch
 * @bug No known bugs.
 * @date June 02, 2019
 * @version 0.1
 */

#include "cell.h"
#include "random.h"
#include "safe_thread.h"


/**
 * @brief : initialize a cell structure
 * @param domain : structure defining limits of game display
 * @param seed : seed needed for reentrant random function
 * @return cell_t* 
 */
cell_t *initialize_cell(domain_t *domain, unsigned int *seed) {
    cell_t *cell = safe_malloc(sizeof(cell_t)); 
    cell->alive = true;
    assign_color(cell, seed);
    assign_new_direction(cell, seed);
    cell->mass = rand_r_min_max(seed,5,8);
    cell->x = rand_r_min_max(seed,1,domain->height-1);
    cell->y = rand_r_min_max(seed,1,domain->width-1);
    return cell;
}

/**
 * @brief : assign a random color to a cell
 * @param cell : cell structure 
 * @param seed : seed needed for reentrant random function
 */
void assign_color(cell_t *cell, unsigned int *seed) {
    cell->color.red = rand_r_min_max(seed,0,255);
    cell->color.green = rand_r_min_max(seed,0,255);
    cell->color.blue = rand_r_min_max(seed,0,255);
}

/**
 * @brief : initialize an array of cells structures
 * @param nb_cells : number of cells of the array
 * @param domain : structure defining limits of game display
 * @param seed : seed needed for reentrant random function
 * @return cell_t** 
 */
cell_t **initialize_cells(int nb_cells, domain_t *domain, unsigned int *seed) {
    cell_t **cells = safe_malloc(nb_cells*sizeof(cell_t*));
    for(int i=0; i<nb_cells;i++) {
        cells[i] = initialize_cell(domain, seed);
    }
    return cells;
}

/**
 * @brief : sets dead status to alive status of a cell depending a propability 
 * @param domain : structure defining limits of game display
 * @param cell : cell structure
 * @param seed : seed needed for reentrant random function
 * @param probability : probability of a cell to respawn
 */
void maybe_reintroduce_cell_in_game(domain_t* domain, cell_t* cell, unsigned int *seed, double probability) {
    double random_number = rand_r_min_max(seed, 0, 100) / 100.0;
    if (random_number <= probability) {
        cell->alive = true;
        cell->x = rand_r_min_max(seed, 1, domain->height-1);
        cell->y = rand_r_min_max(seed, 1, domain->width-1);
        assign_new_direction(cell, seed);
        cell->mass = rand_r_min_max(seed,5,8);
    }
}

/**
 * @brief : sets dead status to alive status of a an array of cells depending a propability
 * @param domain : structure defining limits of game display
 * @param cells : array of cell structures
 * @param cells_number : number of cell structures
 * @param seed : seed needed for reentrant random function
 * @param probability : probability of a cell to respawn
 */
void maybe_reintroduce_cells_in_game(domain_t* domain, cell_t** cells, int cells_number, unsigned int *seed, double probability) {
    for (int i = 0; i < cells_number; i++) {
        if (cells[i]->alive == false) {
            maybe_reintroduce_cell_in_game(domain, cells[i], seed, probability);
        }
    }
}

/**
 * @brief : calculate the new position of a cell on the game board
 * @param c : cell structure
 * @param domain : structure defining limits of game display
 * @param p_change_direction : probability of a cett to change its direction
 * @param seed : seed needed for reentrant random function
 */
void calculate_new_cell_position(cell_t *c, domain_t *domain, double p_change_direction, unsigned int *seed) {
    int velocity = calculate_velocity(c);
    c->x = velocity * c->direction.ux + c->x;
    c->y = velocity * c->direction.uy + c->y;
    handle_wall_colisions(c, domain);
    handle_direction_change(c, p_change_direction, seed);
}

/**
 * @brief : manages the position of a cell when it goes outside the domain
 * @param c : cell structure
 * @param domain : seed needed for reentrant random function
 */
void handle_wall_colisions(cell_t *c, domain_t *domain) {
    if(c->x > domain->height) { 
        c->x = domain->height - c->x % domain->height; 
        c->direction.ux *= -1;
    }
    if(c->y > domain->width) { 
        c->y = domain->width - c->y % domain->width; 
        c->direction.uy *= -1;
    }
    if(c->x < 0) { 
        c->x = -c->x; 
        c->direction.ux *= -1;
    }
    if(c->y < 0) { 
        c->y = -c->y; 
        c->direction.uy *= -1;
    }
}

/**
 * @brief : assign a ramdom new direction to a cell
 * @param c : cell structure
 * @param seed : seed needed for reentrant random function
 */
void assign_new_direction(cell_t *c, unsigned int *seed) {
    do {
        c->direction.ux = rand_r_min_max(seed, -1, 1);
        c->direction.uy = rand_r_min_max(seed, -1, 1);
    } while(c->direction.ux==0 && c->direction.uy==0);
}

/**
 * @brief : change the direction of a cell depending of a probability
 * @param c : cell structure 
 * @param p_change_direction : probability of a cell to change direction
 * @param seed : seed needed for reentrant random function
 */
void handle_direction_change(cell_t *c, double p_change_direction, unsigned int *seed) {
    double p_toss = rand_r_min_max(seed, 0, 100) / 100.0;
        if(p_toss < p_change_direction) {
            assign_new_direction(c, seed);
        }
}

/**
 * @brief : calcule the velocity of a cell 
 * @param c : cell structure
 * @return int
 */
int calculate_velocity(cell_t *c) {
    return (int)(ALPHA + BETA / (1+exp(GAMMA*((double)c->mass-DELTA))));
}

/**
 * @brief : calculate the radius of a cell
 * @param c : cell structure
 * @return double
 */
double calculate_radius(cell_t *c) {
    return 4 + 6 * sqrt(c->mass);
}

/**
 * @brief : free memory of a cell structure
 * @param cells : cell structure
 * @param nb_cells : number of cells in the structure
 */
void free_cells(cell_t **cells, int nb_cells) {
    for( int i=0; i<nb_cells;i++) {
        free(cells[i]);
    }
    free(cells);
}


 

