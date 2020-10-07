/**
 * @file domain.c
 * @brief module to handle domain
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
#include "domain.h"
#include "safe_thread.h"

/**
 * initialize a domain with an height and a width
 * 
 * @param height the height of the domain
 * @param width the width of the domain
 * @return domain_t* 
 */
domain_t* initialize_domain(int height, int width) {
    domain_t* domain = safe_malloc(sizeof(domain_t));
    domain->height = height;
    domain->width = width;
    return domain;
}

