
#include <swilib.h>

__inl
const char * _dlerror()
__def( 0x2F7, const char *)

const char *dlerror(void) {

    return _dlerror();
}
