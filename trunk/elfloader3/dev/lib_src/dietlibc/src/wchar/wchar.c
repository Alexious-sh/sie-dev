#include <string.h>
#include <wchar.h>
#include <errno.h>

size_t wcsrtombs(char *dst, const wchar_t **src, size_t len, mbstate_t *ps)
{
    const char*  s = (const char*)*src;
    const char*  s2 = memchr( s, 0, len );

    if (s2 != NULL)
        len = (s2 - s)+1;

    if (dst != NULL)
        memcpy( dst, s, len );

    *src = (wchar_t*)(s + len);
    return len;
}

size_t wcstombs(char *dst, const wchar_t *src, size_t len)
{
    return wcsrtombs(dst, &src, len, NULL);
}

