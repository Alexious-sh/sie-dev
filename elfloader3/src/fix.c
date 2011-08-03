
#include "fix.h"

/*
long lseek(FILE *fp, int off, int mod, int *err, int *err1 )
{
  fseek(fp, off, mod);
  return ftell(fp);
}
*/


void ShowMSG(int a, int b)
{
  printf("MESSAGE: %s\n", (char*)b);
}
//#define fread(a, b, c, d) read(a, b, c)

void SUBPROC(void *proc, void *p)
{
  ((void(*)(void*))proc)(p);
}
