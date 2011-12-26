

#include <errno.h>

int lstat64(const char *__file, void *__buf)
{
    errno = ENOSYS;
    return -1;
}
