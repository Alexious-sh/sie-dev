
#include <unistd.h>
#include <errno.h>


int dup (int oldfd)
{
    errno = ENOSYS;
    return -1;
}
