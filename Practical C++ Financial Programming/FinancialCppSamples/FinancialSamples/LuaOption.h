//
//  LuaOption.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__LuaOption__
#define __FinancialSamples__LuaOption__

#include "LuaWrap.h"

class Option;

#include <string>

class LuaOption {
public:
    LuaOption(lua_State *l);
    void setObject(lua_State *l);

    static const char className[];
    static LuaWrapper<LuaOption>::RegType methods[];

    // Lua functions should receive lua_State and return int
    int ticker(lua_State *l);
    int strike(lua_State *l);

    int setTicker(lua_State *l);
    int setStrike(lua_State *l);
private:
    Option *m_option;
};

#endif /* defined(__FinancialSamples__LuaOption__) */
