
#include <errno.h>


void *getpwuid ()
{
    errno = ENOSYS;
    return 0;
}


void * getpwnam()
{
    errno = ENOSYS;
    return 0;
}
