
#ifndef __MY_LUA_H__
#define __MY_LUA_H__

#include <swilib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    void (*abort)(int status);
    char keep_alive;
    char *s_folder;
    char *s_name;
    char *script;
    char *plugin_dir;
    char is_aborting;
    char is_lock;
}m_lua_State;


typedef struct
{
  CSM_RAM csm;
  int gui_id;
}LUA_MAIN_CSM;


typedef struct
{
  GUI gui;
  lua_State *L;
  char regfunc[6][128];
  int id;
}LUA_MAIN_GUI;


typedef struct
{
    char curentPen[5];
    char curentBrus[5];
    int curentFont;
    int curentTextAttribyte;
    WSHDR *lua_ws;
    size_t lua_ws_AllocatedSize;
}_lua_draw_engine;

static __inline__ m_lua_State * mluaState(lua_State *L)
{
   lua_getglobal(L, "__MLS__");
   void* point = lua_unboxpointer(L, -1);
   lua_pop(L, 1);
   return (m_lua_State*)point;
}

int docall(lua_State *L, int narg, int clear);
int report(lua_State *L, int status);
int traceback(lua_State *L);


#ifdef __cplusplus
}
#endif

#endif
