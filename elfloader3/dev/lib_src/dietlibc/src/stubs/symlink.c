#include <errno.h>


void *symlink()
{
    errno = ENOSYS;
    return 0;
}
