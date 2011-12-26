
/**
    * 2011
    * (c) Z.Vova
*/


#include <unistd.h>
#include <swilib.h>
#include <errno.h>


ssize_t read(int fd, void *buf, size_t size)
{
    size_t i, err;
    i = _read(fd, buf, size, &err);
    *__errno_location() = err;
    return i;
}
