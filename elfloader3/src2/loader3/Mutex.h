

#ifndef __MUTEX_H__
#define __MUTEX_H__


#define         NU_TASK_SIZE                    42

/* структуры нулеус */
typedef struct NU_TASK_STRUCT
{
    unsigned long   words[NU_TASK_SIZE];
} NU_TASK;


/* нуклеус функции */
extern unsigned int NU_Suspend_Task(NU_TASK *task);
extern unsigned int NU_Resume_Task(NU_TASK *task);
extern void         NU_Sleep(unsigned int);
extern NU_TASK      *NU_Current_Task_Pointer(void);



/* псевдо мютексы */
typedef struct
{
  char is_locked;
  NU_TASK *task;
}Mutex;


int mutex_init(Mutex *mtx);
int mutex_lock(Mutex *mtx);
int mutex_unlock(Mutex *mtx);
int mutex_destroy(Mutex *mtx);

#endif
