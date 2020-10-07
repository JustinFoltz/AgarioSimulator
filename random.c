/**
 * @file random.c
 * @brief module to handle randoms
 *
 * TODO main description
 *
 * @author Lucas Pallud, lucas.pallud@etu.hesge.ch
 * @author Justin Foltz, justin.foltz@etu.hesge.ch
 * @bug No known bugs.
 * @date June 02, 2019
 * @version 0.1
 */
#include <stdlib.h>

/**
 * Generate a random number between min and max included with a given seed 
 * 
 * @param seed the seed we want to generate our random number
 * @param min the min number we want to generate
 * @param max the max number we want to generate 
 * @return int the random number
 */
int rand_r_min_max(unsigned int *seed, int min, int max) {  
    return rand_r(seed) % (max-min+1) + min;
}