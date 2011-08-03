
#ifndef __FIX_H__
#define __FIX_H__

#include <stdio.h>

#define S_SET SEEK_SET
#define S_CUR SEEK_CUR
#define S_END SEEK_END

//long lseek(FILE *fp, int off, int mod, int *err, int *err1 );
void ShowMSG(int a, int b);
void SUBPROC(void *proc, void *p);

#endif