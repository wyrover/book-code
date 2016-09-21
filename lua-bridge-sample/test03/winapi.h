#ifndef __WINAPI_H__
#define __WINAPI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

int luaopen_winapi(lua_State *L);


#ifdef __cplusplus
}
#endif



#endif // __WINAPI_H__