
#include <swihelper.h>
#include <stddef.h>
#include <setjmp.h>


int setjmp(jmp_buf jmpbuf)
{
    __def_noinline( 0x0162, int, jmpbuf)
}


void longjmp(jmp_buf param1, int param2)
{
    __def_noinline( 0x229, void, param1, param2)
}
