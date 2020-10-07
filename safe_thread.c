#include "safe_thread.h"


//création de thread
void safe_pthread_create(pthread_t *thread, void * routine(void* arg), void *arg) {
    if(pthread_create(thread, NULL, routine, arg)) { perror("error thread creation"); exit(EXIT_FAILURE); }
}

//jonture de thread
void safe_pthread_join(pthread_t thread, void **value_ptr) {
    if(pthread_join(thread, value_ptr)) { perror("error thread join"); exit(EXIT_FAILURE); }
}

//initialisation d'un mutex
void safe_pthread_mutex_init(pthread_mutex_t *lock) {
    if(pthread_mutex_init(lock, NULL)) { perror("error thread mutex init"); exit(EXIT_FAILURE); }
}

//acquisition d'un mutex
void safe_pthread_mutex_lock(pthread_mutex_t *mutex) {
    if(pthread_mutex_lock(mutex)) { perror("error thread mutex lock"); exit(EXIT_FAILURE); }
}

//libération d'un mutex
void safe_pthread_mutex_unlock(pthread_mutex_t *mutex) {
    if(pthread_mutex_unlock(mutex)) { perror("error thread mutex unlock"); exit(EXIT_FAILURE); }
}

//destruction d'un mutex
void safe_pthread_mutex_destroy(pthread_mutex_t *mutex) {
    if(pthread_mutex_destroy(mutex)) { perror("error thread mutex destroy"); exit(EXIT_FAILURE); }  
}

//initialisation d'une variable condition
void safe_pthread_cond_init(pthread_cond_t *cond) {
    if(pthread_cond_init(cond, NULL)) { perror("error thread cond init"); exit(EXIT_FAILURE); }
} 

//destruction d'une variable condition
void safe_pthread_cond_destroy(pthread_cond_t *cond) {
    if(pthread_cond_destroy(cond)) { perror("error thread cond destroy"); exit(EXIT_FAILURE); }
}

//mise en sommeil d'un thread et libération du verrou
void safe_pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex) {
    if(pthread_cond_wait(cond, mutex)) { perror("error thread cond wait"); exit(EXIT_FAILURE); }
}

//reveil d'un thread endormi (et acquisition du verrou associé)
void safe_pthread_cond_signal(pthread_cond_t *cond) {
    if(!pthread_cond_signal(cond)) { perror("error thread cond signal"); exit(EXIT_FAILURE); }
}

//initialisation d'une barrière
void safe_pthread_barrier_init(pthread_barrier_t *barrier, unsigned count) {
    if(pthread_barrier_init(barrier, NULL, count)) { perror("error thread barrier init"); exit(EXIT_FAILURE); }
}

//mise en sommeil de threads sur une barrière
void safe_pthread_barrier_wait(pthread_barrier_t *barrier) {
    int wait_status = pthread_barrier_wait(barrier);
    if(wait_status!=0 && wait_status != PTHREAD_BARRIER_SERIAL_THREAD) {
        perror("error thread barrier wait"); exit(EXIT_FAILURE);
    }
}

//reveil des threads endormis sur une barrière
void safe_pthread_barrier_destroy(pthread_barrier_t *barrier) {
    if(pthread_barrier_destroy(barrier)) { perror("error thread barrier destroy"); exit(EXIT_FAILURE); }
}

//safe malloc
void *safe_malloc(size_t __size) {
    void * pointer = malloc(__size);
    if(pointer == NULL) { perror("error malloc"); exit(EXIT_FAILURE); }
    return pointer;
}

