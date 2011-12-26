#include <errno.h>

int chroot(const char *path)
{
    errno = ENOSYS;
    return -1;
}
