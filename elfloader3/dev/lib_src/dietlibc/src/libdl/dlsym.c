
#include <swilib.h>

__inl
void* __dlsym(int handle, const char *name)
__def( 0x2F0, void *, handle, name)


void *dlsym(void *handle, const char *sym)
{
    if(!handle || !sym || !*sym) return 0;

    int d = (int )(*(int*)handle);
    return __dlsym(d, sym);
}
