#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include <string.h>
#ifdef WIN32
#include <io.h>
#include <fcntl.h>
#endif

/* * */

static int LclIoModeSet(
  lua_State *L)

{ // LclIoModeSet
  FILE **StrmPtr = (FILE **) luaL_checkudata(L, 1, LUA_FILEHANDLE);
  if (*StrmPtr) {
    int Bin = 0;
    const char *ModeStr = luaL_checkstring(L, 2);
    if (0 == strcmp("binary", ModeStr)) Bin = 1;
    else if (0 != strcmp("text", ModeStr)) luaL_error(L, "expecting either "
      LUA_QL("binary") " or " LUA_QL("text") " mode");
#ifdef WIN32
    _setmode(_fileno(*StrmPtr), Bin ? _O_BINARY : _O_TEXT);
#endif
  } // if
  else luaL_error(L, "attempt to access a closed file");
  return 0;
} // LclIoModeSet

/* * */

int luaopen_iomode(
  lua_State *L)

{ // luaopen_iomode
  static const luaL_reg Map[] = {
    {"modeset",  LclIoModeSet},
    {NULL, NULL}
  };

  luaL_register(L, LUA_IOLIBNAME, Map);
  return 1;
} // luaopen_iomode
