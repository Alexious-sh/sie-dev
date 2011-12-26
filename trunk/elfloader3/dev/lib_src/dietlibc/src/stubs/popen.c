
#include <stdio.h>
#include <errno.h>


FILE *popen (const char *Command, const char *Mode)
{
    errno = ENOSYS;
    return 0;
}
