#ifndef _SETJMP_H
#define _SETJMP_H

#include <sys/cdefs.h>

__BEGIN_DECLS


#ifndef __JMPBUF_DECL
#define __JMPBUF_DECL
typedef int jmp_buf[11];
#endif

#ifndef __NO_LIBC

int     setjmp(jmp_buf);
void    longjmp(jmp_buf, int);

#endif
__END_DECLS

#endif
