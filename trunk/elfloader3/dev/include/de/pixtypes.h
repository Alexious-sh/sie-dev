
#ifndef __PIXTYPES_H__
#define __PIXTYPES_H__

#include <inttypes.h>

typedef struct
{
    uint8_t r, g, b, a;
}Color;


typedef struct _color
{
    unsigned char a;
    unsigned char r;
    unsigned char g;
    unsigned char b;
} color_t;



typedef struct
{
    unsigned short w, h;
    unsigned int bpnum;
    unsigned char *bitmap;
}image_t;


#endif
