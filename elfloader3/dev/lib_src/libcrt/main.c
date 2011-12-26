#include <swilib.h>
#include <ep3/loader.h>

//#define NEWSTULE



volatile char _curent_dir[128]= {0};

/*
__attribute__((noinline))
void __crt_init_switab()
{
    register int ret=0;
    asm volatile("swi	0x80FF\n\t"
                 "mov	%0, r0"
		     :"=r"(ret)
                     :
                     :"lr");
    __sys_switab_addres = ( int*)ret;
}*/



extern char * _get_folder(char *_buf);
extern void __hcrt_run_initarray(void *_ex);

__attribute__(( noinline ))
char *getcwd(char *buf, size_t max)
{
    strncpy(buf, (char*)_curent_dir, max);
    return buf;
}



static void __crt_run_initarray()
{
    extern void *__ex;
    volatile void *ex = &__ex;
    __hcrt_run_initarray((void*)ex);
}


char *__argv[2] = {0};
int _start(char *exe, char *fname, void *p1)
{
    /* скопипастим путь к эльфу */
    strcpy((char*)_curent_dir, exe);

    /* установим 0x0 после последнего "\\" */
    _get_folder((char*)_curent_dir);

    __crt_run_initarray();
    
#ifdef NEWSTULE /* warning: doesnt work */
    __argv[0] = exe;
    *(int*)(__argv+1) = *(int*)fname;

    int argc = 0;
    while(__argv[argc]) ++argc;

    return main(argc, __argv);
#else
    /* выполним маин */
    return main(exe, fname, p1);
#endif
}






