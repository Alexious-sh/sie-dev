
#include <swilib.h>
#include <stdlib.h>


__inl
int __dlclose(int handle)
__def( 0x2F1, int, handle)

int dlclose(void * handle)
{
    if(handle)
    {
        int d = (int )(*(int*)handle);
        int ret = __dlclose(d);
        free(handle);
        return ret;
    }
    return -1;
}
