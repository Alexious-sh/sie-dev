

#include <wchar.h>

wchar_t *
wcspbrk(s, set)
	const wchar_t *s;
	const wchar_t *set;
{
	const wchar_t *p;
	const wchar_t *q;

	p = s;
	while (*p) {
		q = set;
		while (*q) {
			if (*p == *q) {
				/* LINTED interface specification */
				return (wchar_t *)p;
			}
			q++;
		}
		p++;
	}
	return NULL;
}

