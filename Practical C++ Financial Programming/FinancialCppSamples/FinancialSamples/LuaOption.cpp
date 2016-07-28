//
//  LuaOption.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "LuaOption.h"
#include "Option.h"

#include <lauxlib.h>

#include <stdio.h>

const char LuaOption::className[] = "Option";

LuaOption::LuaOption(lua_State *L)
{
    printf(" called constructor ");fflush(stdout);
//    m_option = (Option*)lua_touserdata(L, 1);
    m_option = new Option("", 10);
    printf(" called constructor 2 %p", m_option);fflush(stdout);
}

void LuaOption::setObject(lua_State *L)
{
    m_option = (Option*)lua_touserdata(L, 1);
}

int LuaOption::ticker(lua_State *L)
{
    lua_pushstring(L, m_option->ticker().c_str());
    return 1;
}

int LuaOption::strike(lua_State *L)
{
    lua_pushnumber(L, m_option->strike());
    return 1;
}

int LuaOption::setTicker(lua_State *L)
{
    printf("on set ticker");
    m_option->setTicker((const char*)luaL_checkstring(L, 1));
    return 0;
}

int LuaOption::setStrike(lua_State *L)
{
    m_option->setStrike((double)luaL_checknumber(L, 1));
    return 0;
}

#define method(class, name) {#name, &class::name}
LuaWrapper<LuaOption>::RegType LuaOption::methods[] = {
    method(LuaOption, ticker),
    method(LuaOption, strike),
    method(LuaOption, setTicker),
    method(LuaOption, setStrike),
    {0,0}
};
