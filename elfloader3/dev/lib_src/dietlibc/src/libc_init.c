
#include <swilib.h>
#include <stdlib.h>


/* === on exit shared lib === */
extern void __libc_exit(int);
void _fini()
{
    //exit(0);
    __libc_exit(0);
}



/* === on init ==========*/
void _init()
{

}
