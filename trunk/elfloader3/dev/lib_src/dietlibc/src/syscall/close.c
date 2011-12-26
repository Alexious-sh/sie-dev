
/**
    * 2011
    * (c) Z.Vova
*/


#include <unistd.h>
#include <swilib.h>
#include <errno.h>


int close(int fd)
{
    size_t i, err;
    i = _close(fd, &err);
    *__errno_location() = err;
    return i;
}
