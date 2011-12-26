#include "dietfeatures.h"
#include <string.h>
#include <swihelper.h>



#ifndef __BUILDIN_FUNCTIONS
/* gcc is broken and has a non-SUSv2 compliant internal prototype.
 * This causes it to warn about a type mismatch here.  Ignore it. */
char *strncat(char *s, const char *t, size_t n) {
  char *dest=s;
  register char *max;
  s+=strlen(s);
  if (__unlikely((max=s+n)==s)) goto fini;
  for (;;) {
    if (__unlikely(!(*s = *t))) break; if (__unlikely(++s==max)) break; ++t;
#ifndef WANT_SMALL_STRING_ROUTINES
    if (__unlikely(!(*s = *t))) break; if (__unlikely(++s==max)) break; ++t;
    if (__unlikely(!(*s = *t))) break; if (__unlikely(++s==max)) break; ++t;
    if (__unlikely(!(*s = *t))) break; if (__unlikely(++s==max)) break; ++t;
#endif
  }
  *s=0;
fini:
  return dest;
}
#else
char *strncat(char *s, const char *t, size_t n) {
    __def_noinline(0x0114, char *, s, t, n)
}

#endif
