#include <swilib.h>
#include <ep3/loader.h>



char * _get_folder(char *_buf)
{
    if( !_buf ) return 0;

    char *si = _buf;
    si = strrchr(si, '\\');
    if( si <= _buf || !*si) return 0;
    ++si;
    *si = 0;
    return _buf;
}


void __hcrt_run_initarray(void *_ex)
{
    Elf32_Exec *ex = (Elf32_Exec*)_ex;

    size_t sz = ex->dyn[DT_INIT_ARRAYSZ] / sizeof(void*);
    void ** arr = (void**)(ex->body + ex->dyn[DT_INIT_ARRAY] - ex->v_addr);

    size_t i;
    for(i=0; i < sz; ++i)
    {
        ( (void (*)())arr[i])();
    }
}


extern void dump_registers();
int registers[16];
int position;
MUTEX mutex;

int func_abort()
{
    asm volatile("push  {r12}\n"
                 "ldr   r12, =position\n"
                 "str   lr, [r12]\n"
                 "pop   {r12}\n"
                 :
                 :
                 :);


    dump_registers();

    /* нехер ломится без очереди */
    MutexLock(&mutex);
    LockSched();

    char _buf[5*1024], *buf = _buf;
    unsigned int err;


    int io = _open("0:\\swilog.txt", A_Create | A_Append/*A_Truncate*/ | A_WriteOnly | A_BIN, P_WRITE, &err);
    if(io==-1) return 0;


    int size = sprintf(buf, " [+] FATAL !!!Syscall error!!!\n"
                       "called from: 0x%X\n"
                       "CepId: %X\n"
                       "register dump: \n"
                       "   r0 - %X\n   r1 - %X\n   r2 - %X\n   r3 - %X\n"
                       /*"   r4 - %X\n   r5 - %X\n"
                       "   r6 - %X\n   r7 - %X\n   r8 - %X\n   r9 - %X\n   r10 - %X\n   r11 - %X\n"
                       "   r12 - %X\n   r13 - %X\n   r14 - %X\n   r15 - %X\n"*/
                       " [i] Crash code dumb:\n\n",
                       position,
                       GBS_GetCurCepid(),
                       registers[0],
                       registers[1],
                       registers[2],
                       registers[3]
                       /*registers[4],
                       registers[5],
                       registers[6],
                       registers[7],
                       registers[8],
                       registers[9],
                       registers[10],
                       registers[11],
                       registers[12],
                       registers[13],
                       registers[14],
                       registers[15]*/
                       );
    position += 4;
    buf += size;
    memcpy(buf, (void*)(position-56), 56); buf += 56;
    memcpy(buf, "\n\n", 2); buf  += 2;

    _write(io, _buf, buf-_buf, &err);
    //_flush(io, &err);
    _close(io, &err);

    MutexUnlock(&mutex);
    UnlockSched();
    return 0;
}


void __onstart()
{
    /* инициализруем переменную __sys_switab_addres */
    extern void *__ex;
    int *table = ((Elf32_Exec*)&__ex)->switab;
    int i;

    for( i=0; i<4096; ++i )
    {
        if((unsigned int)table[i] == 0xFFFFFFFF)
            ((int *)__sys_switab_addres)[i] = (int)&func_abort;
        else
            ((int *)__sys_switab_addres)[i] = table[i];
    }

    MutexCreate(&mutex);
}


void __onclose()
{
    MutexDestroy(&mutex);
}





