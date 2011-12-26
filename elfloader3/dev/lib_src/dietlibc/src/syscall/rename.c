
/**
    * 2011
    * (c) Z.Vova
*/


#include <unistd.h>
#include <swilib.h>
#include <errno.h>



int rename(const char *old, const char *new)
{
    return fmove(old, new, (unsigned int*)__errno_location());
}
