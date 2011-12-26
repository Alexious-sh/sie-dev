
/**
    * 2011
    * (c) Z.Vova
*/


#include <unistd.h>
#include <swilib.h>
#include <errno.h>


int setenv(const char *name, const char *value, int replace)
{
    __def_noinline( 0x2F2, int, name, value, replace)
}


int unsetenv(const char *name)
{
    __def_noinline( 0x2F3, int, name)
}


char * getenv(const char *var)
{
    __def_noinline( 0x2F4, char *, var)
}


int clearenv(void)
{
    __def_noinline( 0x2F5, int)
}

