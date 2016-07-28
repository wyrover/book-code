//
//  LuaWrapper.h
// original code from luna wrapper example (from http://lua-users.org/wiki/LunaWrapper)
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__Luna__
#define __FinancialSamples__Luna__

#include <lua.h>

#include <stdio.h>

template<class T> class LuaWrapper {
public:
    static void Register(lua_State *L) {
        lua_pushcfunction(L, &LuaWrapper<T>::constructor);
        lua_setglobal(L, T::className);

        luaL_newmetatable(L, T::className);
        lua_pushstring(L, "__gc");
        lua_pushcfunction(L, &LuaWrapper<T>::gc_obj);
        lua_settable(L, -3);
    }

    static int constructor(lua_State *L) {
        printf("constructor called");fflush(stdout);
        T* obj = new T(L);

        printf("constructor called6");fflush(stdout);
        lua_newtable(L);
        lua_pushnumber(L, 0);
        printf("constructor called5");fflush(stdout);
        T** a = (T**)lua_newuserdata(L, sizeof(T*));
        *a = obj;
        printf("constructor called4");fflush(stdout);
        luaL_getmetatable(L, T::className);
        lua_setmetatable(L, -2);
        lua_settable(L, -3); // table[0] = obj;

        printf("constructor called2");fflush(stdout);

        for (int i = 0; T::methods[i].name; i++) {
            lua_pushstring(L, T::methods[i].name);
            lua_pushnumber(L, i);
            lua_pushcclosure(L, &LuaWrapper<T>::thunk, 1);
            lua_settable(L, -3);
        }
        printf("constructor called3");fflush(stdout);

        return 1;
    }

    static int thunk(lua_State *L) {
        int i = (int)lua_tonumber(L, lua_upvalueindex(1));
        lua_pushnumber(L, 0);
        lua_gettable(L, 1);

        T** obj = static_cast<T**>(luaL_checkudata(L, -1, T::className));
        lua_remove(L, -1);
        return ((*obj)->*(T::methods[i].mfunc))(L);
    }

    static int gc_obj(lua_State *L) {
        T** obj = static_cast<T**>(luaL_checkudata(L, -1, T::className));
        delete (*obj);
        return 0;
    }

    struct RegType {
        const char *name;
        int(T::*mfunc)(lua_State*);
    };
};



#endif /* defined(__FinancialSamples__LuaWrapper__) */
