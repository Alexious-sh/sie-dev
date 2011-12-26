
#include <stddef.h>
#include <stdlib.h>

int munmap(void *adr, size_t size)
{
    free(adr);
    return 0;
}
