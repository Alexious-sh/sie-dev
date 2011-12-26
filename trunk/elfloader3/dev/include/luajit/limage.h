
#ifndef __LIMAGE_H__
#define __LIMAGE_H__


IMGHDR* pushimage (lua_State *L, IMGHDR *im);
IMGHDR* checkimage (lua_State *L, int index);
void DrwImg(int x, int y, int w, int h, IMGHDR *img);
void FreeIMGHDR(IMGHDR *img);


#endif

