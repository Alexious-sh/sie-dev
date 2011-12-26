
/**
    * 2011
    * (c) Z.Vova
*/


#include <unistd.h>
#include <swilib.h>
#include <errno.h>



long lseek(int _fd, long offset, int whence)
{
    size_t i, err;
    i = _lseek(_fd, offset, whence, &err, &err);
    *__errno_location() = err;
    return i;
}
