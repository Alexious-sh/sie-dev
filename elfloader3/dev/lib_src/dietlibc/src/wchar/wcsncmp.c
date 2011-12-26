
#include <wchar.h>


int
wcsncmp(const wchar_t *s1, const wchar_t *s2, size_t n)
{

        if (n == 0)
                return (0);
        do {
                if (*s1 != *s2++) {
                        /* XXX assumes wchar_t = int */
                        return (*(const unsigned int *)s1 -
                            *(const unsigned int *)--s2);
                }
                if (*s1++ == 0)
                        break;
        } while (--n != 0);
        return (0);
}
