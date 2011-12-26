#include "dietfeatures.h"
#include <string.h>
#include <swihelper.h>

#ifndef __BUILDIN_FUNCTIONS
char* strcat(register char* s,register const char* t)
{
  char *dest=s;
  s+=strlen(s);
  for (;;) {
    if (!(*s = *t)) break; ++s; ++t;
#ifndef WANT_SMALL_STRING_ROUTINES
    if (!(*s = *t)) break; ++s; ++t;
    if (!(*s = *t)) break; ++s; ++t;
    if (!(*s = *t)) break; ++s; ++t;
#endif
  }
  return dest;
}
#else

char* strcat(register char* s,register const char* t)
{
    // syscall strcat
    __def_noinline( 23, char*, s, t);
}

#endif
