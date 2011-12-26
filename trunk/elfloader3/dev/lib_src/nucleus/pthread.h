

#ifndef __PTHREAD_H__
#define __PTHREAD_H__

#include <nucleus/nucleus.h>
#include <swilib.h>

#define PTHREAD_MAGIC 0xDEFAEA00
#define DEFAULT_PRIORITY 0x44
#define DEFAULT_STACK_SIZE 4096

typedef struct
{
    size_t stack_size;
    void *stack;
    size_t prio;
}pthread_attr_t;


typedef struct
{

}pthread_mutexattr_t;



typedef struct
{
    MUTEX mtx;
    pthread_mutexattr_t attr;
    char __is_destroyed;
}pthread_mutex_t;



typedef struct
{
    NU_TASK task;
    pthread_attr_t attr;
    char stack_data[DEFAULT_STACK_SIZE];
    void *stack;
    size_t stack_size;
    void *entry;
    int _is_pthread;
}pthread_t;



int pthread_create (pthread_t * __newthread, const pthread_attr_t * __attr,
                    void *(*__start_routine) (void *), void * __arg);
void pthread_exit (void *__retval);
pthread_t pthread_self (void);
int pthread_equal (pthread_t __thread1, pthread_t __thread2);
int pthread_attr_init (pthread_attr_t *__attr);
int pthread_attr_destroy (pthread_attr_t *__attr);
int pthread_attr_getstackaddr (const pthread_attr_t * __attr, void ** __stackaddr);
int pthread_attr_setstackaddr (pthread_attr_t *__attr, void *__stackaddr);
int pthread_attr_getstacksize (const pthread_attr_t * __attr, size_t * __stacksize);
int pthread_attr_setstacksize (pthread_attr_t *__attr, size_t __stacksize);
int pthread_attr_getstack (const pthread_attr_t * __attr,void ** __stackaddr, size_t * __stacksize);
int pthread_attr_setstack (pthread_attr_t *__attr, void *__stackaddr, size_t __stacksize);
int pthread_getconcurrency (void);
int pthread_join (pthread_t __th, void **__thread_return);
void sleep(size_t ticks);
int pthread_destroy(pthread_t *__th); // Not standart

int pthread_mutex_init (pthread_mutex_t *__mutex,
			       const pthread_mutexattr_t *__mutexattr);
/* Destroy a mutex.  */
int pthread_mutex_destroy (pthread_mutex_t *__mutex);
/* Try locking a mutex.  */
int pthread_mutex_trylock (pthread_mutex_t *__mutex);
/* Lock a mutex.  */
int pthread_mutex_lock (pthread_mutex_t *__mutex);
int pthread_mutex_unlock (pthread_mutex_t *__mutex);



#endif

