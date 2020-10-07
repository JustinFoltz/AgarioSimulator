#ifndef _SAFE_THREAD_
#define _SAFE_THREAD_

//#define _XOPEN_SOURCE 600 // X/Open 6, incorporating POSIX 2004

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


void safe_pthread_create(pthread_t *thread, void *routine(void* arg), void *arg);
void safe_pthread_join(pthread_t thread, void **value_ptr);
void safe_pthread_mutex_init(pthread_mutex_t *lock);
void safe_pthread_mutex_lock(pthread_mutex_t *mutex);
void safe_pthread_mutex_unlock(pthread_mutex_t *mutex);
void safe_pthread_mutex_destroy(pthread_mutex_t *mutex);
void safe_pthread_cond_init(pthread_cond_t *cond);
void safe_pthread_cond_destroy(pthread_cond_t *cond);
void safe_pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
void safe_pthread_cond_signal(pthread_cond_t *cond);
void safe_pthread_barrier_init(pthread_barrier_t *barrier, unsigned count);
void safe_pthread_barrier_wait(pthread_barrier_t *barrier);
void safe_pthread_barrier_destroy(pthread_barrier_t *barrier);
void *safe_malloc(size_t __size);

#endif