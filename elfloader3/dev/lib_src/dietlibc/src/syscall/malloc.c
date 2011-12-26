
#include <swihelper.h>
#include <stddef.h>


void *malloc(size_t size)
{
    __def_noinline(20, void*, size);
}
