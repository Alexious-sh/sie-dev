
#include <errno.h>


void *getgrnam(const char* name)
{
    errno = ENOSYS;
    return 0;
}
