#define _POSIX_SOURCE
#define _XOPEN_SOURCE
#include <sys/types.h>
#include <string.h>
#include <swihelper.h>

#ifndef __BUILDIN_FUNCTIONS
void *memmove(void *dst, const void *src, size_t count)
{
  char *a = dst;
  const char *b = src;
  if (src!=dst)
  {
    if (src>dst)
    {
      while (count--) *a++ = *b++;
    }
    else
    {
      a+=count-1;
      b+=count-1;
      while (count--) *a-- = *b--;
    }
  }
  return dst;
}
#else

void *memmove(void *dst, const void *src, size_t count)
{
    __def_noinline(0x0132, void *, dst, src, count)
}

#endif
