
/*
 * Этот файл является частью программы ElfLoader
 * Copyright (C) 2011 by Z.Vova, Ganster
 * Licence: GPLv3
 */

#include "loader.h"



void ElfKill()
{
    extern  void *ELF_BEGIN;
    kill_data(&ELF_BEGIN, (void (*)(void *))mfree_adr());
}

void CreateTask(void *entry);
int main(char *exename, char *fname)
{
    __init_stdio(exename);
    setvbuf(stdout, 0, _IOFBF, 1024);
    printf(" [A] 0x%X\n", &ELF_BEGIN);

#define NAME "0:\\Misc\\elf3\\test.elf"

    // l_setenv("LD_LIBRARY_PATH", "0:\\Misc\\elf3\\");

    printf("loading...\n");
    Elf32_Exec *ex = elfopen( NAME );
    if( ex == 0 )
    {
        _on_exit();
        SUBPROC((void*)ElfKill);
        return 1;
    }

    void (*addr)(const char * argc, char *argv) = (void(*)(const char* argc, char *argv))elf_entry(ex);

    if( !addr )
    {
        printf("entry not found\n");
    }
    else
    {
        fflush(stdout);
        run_INIT_Array(ex);
        //addr( NAME, (char*)0);
        CreateTask((void *)addr);
    }

    //elfclose(ex);
    _on_exit();
    //SUBPROC((void*)ElfKill);
    return 0;
}


#define         NU_PREEMPT                      10
#define         NU_TASK_SIZE                    42
#define         NU_START                        12
typedef void          VOID;
typedef unsigned long           UNSIGNED;
typedef long                    SIGNED;
typedef unsigned char           DATA_ELEMENT;
typedef DATA_ELEMENT            OPTION;
typedef int                     STATUS;
typedef unsigned char           UNSIGNED_CHAR;
typedef char                    CHAR;

typedef unsigned long *         UNSIGNED_PTR;
typedef unsigned char *         BYTE_PTR;
typedef DATA_ELEMENT            BOOLEAN;
typedef unsigned int            UNSIGNED_INT;


typedef struct NU_TASK_STRUCT
{
    UNSIGNED      words[NU_TASK_SIZE];
} NU_TASK;



STATUS  (*NU_Create_Task) (NU_TASK *task, CHAR *name,
                                   VOID (*task_entry)(UNSIGNED, VOID *), UNSIGNED argc,
                                   VOID *argv, VOID *stack_address, UNSIGNED stack_size,
                                   OPTION priority, UNSIGNED time_slice,
                                   OPTION preempt, OPTION auto_start) = (int (*)(NU_TASK *, CHAR *, VOID
          (*)(UNSIGNED, VOID *), UNSIGNED, VOID *, VOID *, UNSIGNED, OPTION,
          UNSIGNED, OPTION, OPTION))0xA00A2420;


/*

 A00A2420 = NU_Create_Task
 A00A253C = NU_Delete_Task
 A00A2598 = NU_Reset_Task
 A00A26B4 = NU_Terminate_Task
 A00A26D8 = NU_Resume_Task
 A00A26FC = NU_Suspend_Task
 A00A2738 = NU_Relinquish
 A00A275C = NU_Sleep
 A00A3AA4 = NU_Change_Priority
 A00A3ABC = NU_Change_Preemption
 A00A3AD8 = NU_Change_Time_Slice


 A00A4274 = NU_Create_Timer
 A00A42E8 = NU_Delete_Timer
 A00A4308 = NU_Reset_Timer
 A00A436C = NU_Control_Timer
 ??? = NU_Established_Timers
 A00A4D0C = NU_Set_Clock     // хз, она ли
 гдето в том же месте, я ничего не понял там = NU_Retrieve_Clock

 */


void CreateTask(void *entry)
{
    NU_TASK *task = malloc(sizeof(NU_TASK));
    char *stack = malloc(1024*4);
    STATUS st = NU_Create_Task(task, "testelf", (void(*)(UNSIGNED,void*))entry, 1,
                               "0:\\Misc\\elf3\\test.elf", stack, 1024*4, 0x84, 0, NU_PREEMPT, NU_START);

    /*STATUS st = NU_Create_Task(0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0);*/
}






