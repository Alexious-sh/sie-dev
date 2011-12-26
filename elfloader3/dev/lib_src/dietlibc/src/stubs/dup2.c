
#include <unistd.h>
#include <errno.h>


int dup2 (int oldfd,int newfd)
{
    errno = ENOSYS;
    return -1;
}
