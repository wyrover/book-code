//
//  LuaMain.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "LuaMain.h"

#include <iostream>

#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "LuaWrap.h"
#include "LuaOption.h"

using std::cout;
using std::cerr;
using std::endl;


int main_lua() {
    char buff[256];

    lua_State *L = luaL_newstate();
    int error;

    // load some of the (C) libraries included with Lua
    luaopen_base(L);
    luaopen_table(L);
    luaopen_io(L);
    luaopen_string(L);
    luaopen_math(L);

    // load LuaOption object
    LuaWrapper<LuaOption>::Register(L);

    while (fgets(buff, sizeof(buff), stdin) != NULL) {
        error = luaL_loadbuffer(L, buff, strlen(buff), "line") ||
        lua_pcall(L, 0, 0, 0);
        if (error) {
            cerr << lua_tostring(L, -1) << endl;
            lua_pop(L, 1);  // remove error from Lua stack
        }
    }

    lua_close(L);
    return 0;
}