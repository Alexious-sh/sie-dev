
#ifndef lualib_h
#define lualib_h

#include "lua.h"


LUALIB_API void luaL_myopenlibs (lua_State *L);

#define LUA_SYSNAME "sys"
LUALIB_API int (luaopen_sys) (lua_State *L);

#define LUA_CSMNAME "csm"
LUALIB_API int luaopen_csm (lua_State *L);

#define LUA_UINAME "ui"
LUALIB_API int (luaopen_ui) (lua_State *L);

#define LUA_FILENAME "file"
LUALIB_API int (luaopen_file) (lua_State *L);

#define LUA_SYSNAME "sys"
LUALIB_API int (luaopen_sys) (lua_State *L);

#define LUA_GUINAME "gui"
LUALIB_API int (luaopen_gui) (lua_State *L);

#define LUA_USTRINGNAME "ustring"
LUALIB_API int luaopen_ustring (lua_State *L);

#define LUA_IMAGENAME "Image"
LUALIB_API int (luaopen_image) (lua_State *L);

#define LUA_TIMENAME "time"
LUALIB_API int (luaopen_time) (lua_State *L);

#define LUA_FILENAME "file"
LUALIB_API int (luaopen_file) (lua_State *L);

#endif
