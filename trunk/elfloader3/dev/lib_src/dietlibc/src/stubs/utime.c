
#include <errno.h>
#include <errno.h>

int utime(const char *f, void *t)
{
    errno = ENOSYS;
    return -1;
}
