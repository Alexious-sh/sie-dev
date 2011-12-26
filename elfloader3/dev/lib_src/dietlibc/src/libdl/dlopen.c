#include <fcntl.h>
#include <dlfcn.h>
#include <limits.h>
#include <swilib.h>
#include <stdlib.h>

__inl
int __dlopen(const char *name)
__def( 0x2EF, int, name)

void *dlopen(const char *filename, int flags)
{
    int a = __dlopen(filename);
    if(a < 0) return 0;

    /* must be address */
    void *d = malloc(4);
    *(int*)d = (int)a;
    return (void*)d;
}
