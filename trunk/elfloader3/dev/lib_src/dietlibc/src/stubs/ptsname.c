
#include <errno.h>


char *ptsname (int fd)
{
    errno = ENOSYS;
    return 0;
}
