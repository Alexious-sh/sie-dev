
#include <swilib.h>
#include "pthread.h"
#include <string.h>



int pthread_create (pthread_t * __newthread, const pthread_attr_t * __attr,
                    void *(*__start_routine) (void *), void * __arg)
{
    STATUS s;

    memset(__newthread, 0, sizeof(pthread_t));

    if(__attr)
    {
        memcpy(&__newthread->attr, __attr, sizeof(pthread_attr_t));
        __newthread->stack = __attr->stack;
        __newthread->stack_size = __attr->stack_size;
        if(!__newthread->stack)
        {
            __newthread->stack = __newthread->stack_data;
            __newthread->stack_size = DEFAULT_STACK_SIZE;
        }
    }else
    {
        __newthread->attr.prio = DEFAULT_PRIORITY;
        __newthread->stack = __newthread->stack_data;
        __newthread->stack_size = DEFAULT_STACK_SIZE;
    }

    __newthread->_is_pthread = PTHREAD_MAGIC;
    __newthread->entry = (void*)__start_routine;

    s = NU_Create_Task ((NU_TASK*)__newthread, 0,
                                   (void (*)(UNSIGNED, void *))__start_routine, (int)__arg,
                                   __arg, __newthread->stack, __newthread->stack_size,
                                   __newthread->attr.prio, 0,
                                   NU_PREEMPT, NU_START);

    return s;
}



void pthread_exit (void *__retval)
{
    pthread_t *_curthread = (pthread_t*)NU_Current_Task_Pointer();

    NU_Terminate_Task(&_curthread->task);
    NU_Delete_Task(&_curthread->task);
}



pthread_t pthread_self (void)
{
    return *(pthread_t*)NU_Current_Task_Pointer();
}


int pthread_equal (pthread_t __thread1, pthread_t __thread2)
{
    return memcmp(&__thread1, &__thread2, sizeof(NU_TASK)) == 0;
}



int pthread_attr_init (pthread_attr_t *__attr)
{
    __attr->stack_size = DEFAULT_STACK_SIZE;
}


int pthread_attr_destroy (pthread_attr_t *__attr)
{
    zeromem(__attr, sizeof(pthread_attr_t));
}



int pthread_attr_getstackaddr (const pthread_attr_t * __attr, void ** __stackaddr)
{
    if(!__attr || !__stackaddr) return -1;
    *__stackaddr = __attr->stack;
    return 0;
}



int pthread_attr_setstackaddr (pthread_attr_t *__attr, void *__stackaddr)
{
    if(!__attr || !__stackaddr) return -1;
    __attr->stack = __stackaddr;
    return 0;
}



int pthread_attr_getstacksize (const pthread_attr_t * __attr, size_t * __stacksize)
{
    if(!__attr || !__stacksize) return -1;
    *__stacksize = __attr->stack_size;
    return 0;
}



int pthread_attr_setstacksize (pthread_attr_t *__attr, size_t __stacksize)
{
    if(!__attr || !__stacksize) return -1;
    __attr->stack_size = __stacksize;
    return 0;
}



int pthread_attr_getstack (const pthread_attr_t * __attr,void ** __stackaddr, size_t * __stacksize)
{
    if(!__attr || !__stackaddr || !__stacksize) return -1;

    *__stackaddr = __attr->stack;
    *__stacksize = __attr->stack_size;

    return 0;
}



int pthread_attr_setstack (pthread_attr_t *__attr, void *__stackaddr, size_t __stacksize)
{
    if(!__attr || !__stackaddr || !__stacksize) return -1;
    __attr->stack      = __stackaddr;
    __attr->stack_size = __stacksize;
    return 0;
}



int pthread_getconcurrency (void)
{
    pthread_t _curthread = pthread_self();
    if(_curthread._is_pthread != (int)PTHREAD_MAGIC) return -1;

    return _curthread.attr.prio;
}


/* stub */
int pthread_setconcurrency (int __level)
{

}

int pthread_join (pthread_t __th, void **__thread_return)
{
    if(__thread_return)
        *__thread_return = (void*)NU_Reset_Task((NU_TASK*)&__th, 0, 0);
    else
        NU_Reset_Task((NU_TASK*)&__th, 0, 0);
    return 0;
}


void sleep(size_t ticks)
{
    NU_Sleep(ticks);
}


int pthread_destroy(pthread_t *__th)
{
    NU_Terminate_Task((NU_TASK*)&__th);
    NU_Delete_Task((NU_TASK*)&__th);
}

/* mutex */

/* Initialize a mutex.  */
int pthread_mutex_init (pthread_mutex_t *__mutex,
			       const pthread_mutexattr_t *__mutexattr)
{
    MutexCreate((MUTEX*)__mutex);
    __mutex->__is_destroyed = 0;
    return 0;
}

/* Destroy a mutex.  */
int pthread_mutex_destroy (pthread_mutex_t *__mutex)
{
    __mutex->__is_destroyed = 1;
    MutexDestroy((MUTEX*)__mutex);
    return 0;
}

/* Try locking a mutex.  */
int pthread_mutex_trylock (pthread_mutex_t *__mutex)
{
    if(__mutex->__is_destroyed) return -1;
    MutexLock((MUTEX*)__mutex);
}

/* Lock a mutex.  */
int pthread_mutex_lock (pthread_mutex_t *__mutex)
{
    if(__mutex->__is_destroyed) return -1;
    pthread_mutex_trylock(__mutex);
}


int pthread_mutex_unlock (pthread_mutex_t *__mutex)
{
    if(__mutex->__is_destroyed) return -1;
    MutexUnlock((MUTEX*)__mutex);
}


