#ifndef _PNG_LIST_H_
#define _PNG_LIST_H_
#include "swilib.h"

typedef struct
{
  void *next;
  char *pngname;
  IMGHDR * img;
}PNGLIST;

typedef struct
{
  void *next;
  int icon;
  IMGHDR * img;
}DYNPNGICONLIST;

typedef struct
{
  PNGLIST * pltop;
  char *bitmap;
  DYNPNGICONLIST *dyn_pltop;
}PNGTOP_DESC;  

#define PNG_8 1
#define PNG_16 2

#if NEWSGOLD || X75
#define PNG_24 3
#endif

#define PNG_1 0xFF

#endif
