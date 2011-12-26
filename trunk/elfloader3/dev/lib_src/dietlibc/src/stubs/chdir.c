
#include <errno.h>


int chdir(const char *patch)
{
    errno = ENOSYS;
    return -1;
}
