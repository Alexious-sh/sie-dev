
/**
    * 2011
    * (c) Z.Vova
*/


#include <unistd.h>
#include <swilib.h>
#include <errno.h>


int truncate(const char *path, off_t size)
{
    int fd = _open(path, A_WriteOnly | A_Create | A_Truncate | A_BIN, P_WRITE, 0);
    if(fd==-1)
    {
        __set_errno(ENOENT);
        return -1;
    }
    int r = _truncate(fd, size, __errno_location());
    _close(fd,0);

    return r;
}

