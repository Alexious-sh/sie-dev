
/**
    * 2011
    * (c) Z.Vova
*/


#include <unistd.h>
#include <swilib.h>
#include <errno.h>


int ftruncate(int fd, off_t size)
{
    return setfilesize(fd, size, (unsigned int*)__errno_location());
}
