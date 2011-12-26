/*
** $Id: lualib.h,v 1.36.1.1 2007/12/27 13:02:25 roberto Exp $
** Lua standard libraries
** See Copyright Notice in lua.h
*/


#ifndef lualib_h
#define lualib_h

#include "lua.h"

#ifdef __cplusplus
extern "C" {
#endif

LUALIB_API void luaL_openlibs (lua_State *L);

/* Key to file-handle type */
#define LUA_FILEHANDLE		"FILE*"

#define LUA_COLIBNAME	"coroutine"
LUALIB_API int (luaopen_base) (lua_State *L);

#define LUA_TABLIBNAME	"table"
LUALIB_API int (luaopen_table) (lua_State *L);

#define LUA_IOLIBNAME	"io"
LUALIB_API int (luaopen_io) (lua_State *L);

#define LUA_OSLIBNAME	"os"
LUALIB_API int (luaopen_os) (lua_State *L);

#define LUA_STRLIBNAME	"string"
LUALIB_API int (luaopen_string) (lua_State *L);

#define LUA_MATHLIBNAME	"math"
LUALIB_API int (luaopen_math) (lua_State *L);

#define LUA_DBLIBNAME	"debug"
LUALIB_API int (luaopen_debug) (lua_State *L);

#define LUA_LOADLIBNAME	"package"
LUALIB_API int (luaopen_package) (lua_State *L);

#define LUA_TIMENAME "time"
LUALIB_API int (luaopen_time) (lua_State *L);

#define LUA_UINAME "ui"
LUALIB_API int (luaopen_ui) (lua_State *L);

#define LUA_FILENAME "file"
LUALIB_API int (luaopen_file) (lua_State *L);

#define LUA_SYSNAME "sys"
LUALIB_API int (luaopen_sys) (lua_State *L);

#define LUA_GUINAME "gui"
LUALIB_API int (luaopen_gui) (lua_State *L);

#define LUA_IMAGENAME "Image"
LUALIB_API int (luaopen_image) (lua_State *L);

#define LUA_USTRINGNAME "ustring"
LUALIB_API int luaopen_ustring (lua_State *L);

#define LUA_CSMNAME "csm"
LUALIB_API int luaopen_csm (lua_State *L);

#define LUA_STRINGLISTNAME "StringList"
LUALIB_API int luaopen_slist (lua_State *L);

int lua_isExistArg(lua_State *L, int n, char *msg);

#ifndef lua_assert
#define lua_assert(x)	((void)0)
#endif

#ifdef __cplusplus
}
#endif

#endif
