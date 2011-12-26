
#include <stdio.h>

void abort()
{
    fprintf(stderr, "ATTENTION: called to abort!\n");
}
