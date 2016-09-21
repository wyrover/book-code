#ifndef lua_cjson_h__
#define lua_cjson_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

int luaopen_cjson(lua_State *l);
int luaopen_cjson_safe(lua_State *l);

#ifdef __cplusplus
}
#endif




#endif // lua_cjson_h__
