
#include <swilib.h>
#include <stddef.h>
#include <stdlib.h>


void* mmap( void*  addr,  size_t  size, int  prot, int  flags, int  fd,  long  offset )
{
    if( fd == -1 && !addr)
    {
        char *a = malloc(size);
        return a;
    }
    ShowMSG(1, (int)"Bad mmap");

    return 0;
}


void* mmap64(void *__addr, size_t __len, int __prot,
		     int __flags, int __fd, off64_t __offset)
{
    return mmap(__addr, __len, __prot, __flags, __fd, __offset);
}
