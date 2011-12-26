/**
    * 2011
    * (c) Z.Vova
*/


#include <unistd.h>
#include <swilib.h>
#include <errno.h>


int rmdir(const char *dir)
{
    return _rmdir(dir, (unsigned int*)__errno_location());
}

