
#include <errno.h>
#include <sys/types.h>


int
readlink (const char *path, char *buf, size_t bufsize)
{
    __set_errno (ENOSYS);
    return -1;
}

