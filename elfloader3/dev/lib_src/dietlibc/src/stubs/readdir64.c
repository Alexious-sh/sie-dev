
#include <errno.h>


void *readdir64()
{
    errno = ENOSYS;
    return 0;
}
