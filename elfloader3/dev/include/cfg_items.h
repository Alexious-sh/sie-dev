

#ifndef __CFG_ITEMS_H__
#define __CFG_ITEMS_H__

#define CFG_UINT 1
#define CFG_STR_UTF8 2
#define CFG_INT 3
#define CFG_STR_WIN1251 4
#define CFG_CBOX 5
#define CFG_STR_PASS 6
#define CFG_COORDINATES 7
#define CFG_COLOR 8
//Level
//min<>0 - start tag (name used)
//min=0 - end tag
#define CFG_LEVEL 9

#define CFG_UTF8_STRING 10
#define CFG_CHECKBOX 11
#define CFG_TIME 12
//TTime
#define CFG_DATE 13
//TDate
#define CFG_RECT 14
//RECT
//#define CFG_STR_GB 15
#define CFG_UTF8_STRING_PASS 16
#define CFG_KEYCODE 17

typedef struct
{
  int type;
  char name[32];
  unsigned int min;
  unsigned int max;
}CFG_HDR;

typedef struct
{
  char cbox_text[32];
}CFG_CBOX_ITEM;

/*
  (c) Z.Vova
*/

#define __DATA_CFG_BEGIN(x) ((void*)(&__config_begin##x)+4)
#define __DATA_CFG_END(x) ((void*)(&__config_end##x))

#define __root __attribute__((section("CONFIG"), used)) volatile
#define __CFG_BEGIN(x) __root int __config_begin##x = 0L;
#define __CFG_END(x) __root int __config_end##x = 0L;

#define __CONFIG(x, cfg) &cfg, (int*)&__config_begin##x, (int*)&__config_end##x

#define __CONFIG_EXTERN(x, cfg) extern CFG_HDR cfg; \
                            extern int __config_begin##x; \
                            extern int __config_end##x;


#endif
