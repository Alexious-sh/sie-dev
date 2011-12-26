

#ifndef __PIXMAP_H__
#define __PIXMAP_H__

#include <inttypes.h>
#include "pixtypes.h"


#define stupidTest(x) (!x || !x->bitmap)


uint32_t alphaBlend(uint32_t c1, uint32_t c2, uint8_t alpha);

/* pixel manipulation */
static inline uint16_t rgb32ToRgb16(const uint32_t rgb32) {
    return (rgb32 >> 8 & 0xF800) | (rgb32 >> 5 & 0x07E0) | (rgb32 >> 3 & 0x001F);
}


static inline uint32_t rgb16ToRgb32(uint16_t c) {
    return 0xff000000
        | ((((c) << 3) & 0xf8) | (((c) >> 2) & 0x7))
        | ((((c) << 5) & 0xfc00) | (((c) >> 1) & 0x300))
        | ((((c) << 8) & 0xf80000) | (((c) << 3) & 0x70000));
}


static inline void rgb16To32(uint8_t dst[4], uint16_t col, uint8_t alpha) {
    dst[0] = (col&0x1F)<<3;
    dst[1] = (col&0x7E0)>>3;
    dst[2] = (col&0xF800)>>8;
    dst[3] = alpha;
}

static inline uint32_t rgb24ToRgb32(uint32_t col, uint8_t alpha) {

    uint32_t c32_o = 0;
    uint8_t *c24 = (uint8_t*)&col;
    uint8_t *c32 = (uint8_t*)&c32_o;
    c32[0] = c24[2];
    c32[1] = c24[1];
    c32[2] = c24[0];
    c32[3] = alpha;
    return c32_o;
}


static inline uint32_t rgb32ToRgb24(uint32_t col) {

    uint32_t c24_o = 0;
    uint8_t *c32 = (uint8_t*)&col;
    uint8_t *c24 = (uint8_t*)&c24_o;
    c24[0] = c32[2];
    c24[1] = c32[1];
    c24[2] = c32[0];

    return c24_o;
}


static inline uint32_t rgbTo32(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return (a << 24) | (r << 16) | (g << 8) | b;
}


static inline uint16_t rgbTo16(uint8_t r, uint8_t g, uint8_t b) {
    return ((( r >> 3 ) << 11 ) | (( g >> 2 ) << 5) | (b >> 3) );
}


static inline void rgb32toRgb(uint32_t c, uint8_t dst[4]) {
    uint8_t a = (c >> 24) & 0xff;
    uint8_t r = (c >> 16) & 0xff;
    uint8_t g = (c >> 8) & 0xff;
    uint8_t b = c & 0xff;

    dst[0] = r;
    dst[1] = g;
    dst[2] = b;
    dst[3] = a;
}

#ifdef __cplusplus
extern "C"
#endif
//int convertBitsNumber(uint8_t num);

#endif
