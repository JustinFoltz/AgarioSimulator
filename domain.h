/**
 * @file domain.h
 * @brief header of module domain
 *
 * TODO main description
 *
 * @author Lucas Pallud, lucas.pallud@etu.hesge.ch
 * @author Justin Foltz, justin.foltz@etu.hesge.ch
 * @bug No known bugs.
 * @date June 02, 2019
 * @version 0.1
 */

#ifndef _DOMAIN_
#define _DOMAIN_

typedef struct {
    int height;
    int width;
} domain_t;


domain_t* initialize_domain(int height, int width);
#endif