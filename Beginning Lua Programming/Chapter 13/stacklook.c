#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include <stdio.h>

#define STACKOP(Code) (Code), LclStackPrint(L, #Code)

/* * */

static void LclStackPrint(
  lua_State *L,
  const char * Str)

{ // LclStackPrint
  int J, Top;

  printf("%-26s [", Str);
  Top = lua_gettop(L);
  for (J = 1; J <= Top; J++) {
    if (lua_isnil(L, J)) printf(" - ");
    else printf(" %d ", lua_tointeger(L, J));
  } // J
  printf("]\n");
} // LclStackPrint

/* * */

static int LclStackLook(
  lua_State *L)

  // three integers <- stack.look(zero or more integers)

{ // LclStackLook
  int J, Top;

  for (J = 1, Top = lua_gettop(L); J <= Top; J++)
    luaL_checkinteger(L, J);
  LclStackPrint(L, "Initial stack");
  STACKOP(lua_settop(L, 3));
  STACKOP(lua_settop(L, 5));
  STACKOP(lua_pushinteger(L, 5));
  STACKOP(lua_pushinteger(L, 4));
  STACKOP(lua_replace(L, -4));
  STACKOP(lua_replace(L, 5));
  STACKOP(lua_remove(L, 3));
  STACKOP(lua_pushinteger(L, 3));
  STACKOP(lua_insert(L, -3));
  STACKOP(lua_pushvalue(L, 2));
  STACKOP(lua_pop(L, 1));
  return 3;
} // LclStackLook

/* * */

int luaopen_stacklook(
  lua_State *L)

{ // luaopen_stacklook
  static const luaL_reg Map[] = {
    {"look",  LclStackLook},
    {NULL, NULL}
  };

  luaL_register(L, "stack", Map);
  return 1;
} // luaopen_stacklook
