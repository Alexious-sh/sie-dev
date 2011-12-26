
#include <swilib.h>
/*
__attribute__((noinline))
void zeromem_a (void * dest, int n)
{
    zeromem(dest,n);
}

__attribute__((noinline))
void memcpy_a (void *dest,const void *source,int cnt)
{
    memcpy(dest,source,cnt);
}
*/
__attribute__((noinline))
char* strncpy_a (char *dest,const char *source,int maxlen)
{
    return strncpy(dest,source,maxlen);
}

