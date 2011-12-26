
/**
    * 2011
    * (c) Z.Vova
*/


#include <unistd.h>
#include <swilib.h>
#include <errno.h>


ssize_t write(int fd, const void *buf, size_t size)
{
    size_t i, err;
    i = _write(fd, buf, size, &err);
    *__errno_location() = err;
    return i;
}
