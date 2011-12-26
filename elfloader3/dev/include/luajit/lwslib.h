

#ifndef __LWSLIB_H__
#define __LWSLIB_H__

#include "lua.h"
#include "lauxlib.h"

#define toUstringp(L, idx)	((WSHDR *)luaL_checkudata(L, idx, USTRINGHANDLE))
void DecodeWs2Str(WSHDR* wsc, luaL_Buffer *b, int slen);

WSHDR* pushustring (lua_State *L, WSHDR *im);
WSHDR* checkustring (lua_State *L, int index);


#endif
