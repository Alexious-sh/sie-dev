
/**
  (c) Z.Vova
*/


#include "loader.h"
#include "Mutex.h"


__arm int mutex_init(Mutex *mtx)
{
  /* шо это нам подсунули? */
  if(!mtx) return -1;
  zeromem(mtx, sizeof(Mutex));
  
  NU_TASK *task = NU_Current_Task_Pointer();
  
  /* а такое может быть? */
  if( !task ) return -2;
  
  /* инициализируем структуру */
  mtx->task = task;
  mtx->is_locked = 0;
  return 0;
}



__arm int mutex_lock(Mutex *mtx)
{
  if(!mtx) return -1;
  
  if(!mtx->task) return -2;
  
  /* ы, локанули и ещЄ раз чтоль? о_ќ*/
  if(mtx->is_locked) return -3;
  
  unsigned int status = 0;
  //status = NU_Suspend_Task(mtx->task);
  mtx->is_locked = 1;
  
  while(mtx->is_locked)
  {
    if(!mtx->is_locked) break;
    NU_Sleep(5);
  }
  
  return status;
}




__arm int mutex_unlock(Mutex *mtx)
{
  if(!mtx) return -1;
  if(!mtx->task) return -2;
  if(!mtx->is_locked) return -3;
  unsigned int status = 0;
  //status = NU_Resume_Task(mtx->task);
  mtx->is_locked = 0;
  return status;
}


__arm int mutex_destroy(Mutex *mtx)
{
  if(!mtx) return -1;
  
  if(mtx->is_locked)
  {
    mtx->is_locked = 0;
    //NU_Resume_Task(mtx->task);
  }
  
  zeromem(mtx, sizeof(Mutex));
  return 0;
}

