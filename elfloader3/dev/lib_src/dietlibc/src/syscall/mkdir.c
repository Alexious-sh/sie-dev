/**
    * 2011
    * (c) Z.Vova
*/


#include <unistd.h>
#include <swilib.h>
#include <errno.h>


int mkdir(const char *dir)
{
    return _mkdir(dir, (unsigned int*)__errno_location());
}
