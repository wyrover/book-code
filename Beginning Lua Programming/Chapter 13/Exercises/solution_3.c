#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

/* * */

static int BitOr(
  lua_State *L)

{ // BitOr
  int Val, J, Top;

  Top = lua_gettop(L);
  Val = 0;
  for (J = 1; J <= Top; J++) Val |= luaL_checkinteger(L, J);
  lua_pushinteger(L, Val);
  return 1;
} // BitOr

/* * */

static int BitAnd(
  lua_State *L)

{ // BitAnd
  int Val, J, Top;

  Top = lua_gettop(L);
  Val = Top > 0 ? -1 : 0;
  for (J = 1; J <= Top; J++) Val &= luaL_checkinteger(L, J);
  lua_pushinteger(L, Val);
  return 1;
} // BitAnd

/* * */

int luaopen_bit(
  lua_State *L)

{ // luaopen_bit
  static const luaL_reg Map[] = {
    {"_and",  BitAnd},
    {"and",  BitAnd},
    {"_or",  BitOr},
    {"or",  BitOr},
    {NULL, NULL}
  };

  luaL_register(L, "bit", Map);
  return 1;
} // luaopen_bit
