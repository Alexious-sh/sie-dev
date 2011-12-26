
#include <errno.h>

int stat64 (const char *file, void *buf)
{
    errno = ENOSYS;
    return -1;
}


int fstat64(int fildes, void *buf)
{
    errno = ENOSYS;
    return -1;
}
