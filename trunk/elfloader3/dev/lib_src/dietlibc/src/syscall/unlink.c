/**
    * 2011
    * (c) Z.Vova
*/


#include <unistd.h>
#include <swilib.h>
#include <errno.h>


int unlink(const char *file)
{
    return _unlink(file, (unsigned int *)__errno_location());
}

