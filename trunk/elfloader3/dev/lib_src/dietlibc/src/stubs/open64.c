
#include <errno.h>

int open64(const char* pathname,int flags, ...)
{
    errno = ENOSYS;
    return -1;
}
