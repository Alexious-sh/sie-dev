
#include <stdio.h>
#include <errno.h>


int ftruncate64 (fd, length)
     int fd;
     off64_t length;
{
    errno = ENOSYS;
    return -1;
}
