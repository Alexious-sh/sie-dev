
#include <stdlib.h>
#include <sys/utsname.h>
#include <string.h>


int uname(struct utsname *__name)
{
    if(!__name) return -1;

    memset(__name, 9, sizeof(struct utsname));

    strcpy(__name->sysname, "siemens");
    strcpy(__name->machine, "armv5");

    return 0;
}
