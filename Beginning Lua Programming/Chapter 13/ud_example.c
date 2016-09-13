#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include <stdio.h>

#define CnExampleStr "example"

typedef struct {
  int Val;
  int Open;
} ExampleType, * ExamplePtrType;

/* * */

static ExamplePtrType LclExamplePtrGet(
  lua_State *L,
  int StkPos)

  /* Returns example pointer if value at position StkPos is valid and open,
  otherwise an error is generated. */

{ // LclExamplePtrGet
  ExamplePtrType ExamplePtr = luaL_checkudata(L, StkPos, CnExampleStr);
  if (! ExamplePtr->Open)
    luaL_error(L, "attempt to use a closed " CnExampleStr);
  return ExamplePtr;
} // LclExamplePtrGet

/* * */

static int LclExampleStr(
  lua_State *L)

  // "example" <- tostring(ExampleHnd)

{ // LclExampleStr
  ExamplePtrType ExamplePtr;

  // Stk: ExampleHnd?
  ExamplePtr = luaL_checkudata(L, 1, CnExampleStr);
  if (ExamplePtr->Open)
    lua_pushfstring(L, CnExampleStr " (%d)", ExamplePtr->Val);
  else lua_pushfstring(L, CnExampleStr " (%d, closed)", ExamplePtr->Val);
  // Stk: ExampleHnd IdStr
  return 1;
} // LclExampleStr

/* * */

static int LclExampleGet(
  lua_State *L)

  // Val <- ExampleHnd:get()

{ // LclExampleGet
  ExamplePtrType ExamplePtr = LclExamplePtrGet(L, 1);
  // Stk: ExampleHnd
  lua_pushnumber(L, ExamplePtr->Val);
  printf("Retrieving value of " CnExampleStr " (%d)\n", ExamplePtr->Val);
  // Stk: ExampleHnd Val
  return 1;
} // LclExampleGet

/* * */

static int LclExampleSet(
  lua_State *L)

  // OldVal <- ExampleHnd:set(NewVal)

{ // LclExampleSet
  int Val;
  // Stk: ExampleHnd? NewVal?
  ExamplePtrType ExamplePtr = LclExamplePtrGet(L, 1);
  // Stk: ExampleHnd NewVal?
  Val = luaL_checkint(L, 2);
  // Stk: ExampleHnd NewVal
  printf("Setting value of " CnExampleStr " from %d to %d\n",
    ExamplePtr->Val, Val);
  lua_pushnumber(L, ExamplePtr->Val);
  // Stk: ExampleHnd NewVal OldVal
  ExamplePtr->Val = Val;
  return 1;
} // LclExampleSet

/* * */

static int LclExampleClose(
  lua_State *L)

  // ExampleHnd:close()

{ // LclExampleClose
  ExamplePtrType ExamplePtr = LclExamplePtrGet(L, 1);
  printf("Closing " CnExampleStr " (%d) explicitly\n", ExamplePtr->Val);
  ExamplePtr->Open = 0;
  return 0;
} // LclExampleClose

/* * */

static int LclExampleGc(
  lua_State *L)

  // metatable(ExampleHnd).__gc(ExampleHnd)

{ // LclExampleGc
  ExamplePtrType ExamplePtr = luaL_checkudata(L, 1, CnExampleStr);
  if (ExamplePtr->Open) {
    printf("Collecting and closing " CnExampleStr " (%d)\n",
      ExamplePtr->Val);
    ExamplePtr->Open = 0;
  } // if
  else printf("Collecting " CnExampleStr " (%d), already closed\n",
    ExamplePtr->Val);
  return 0;
} // LclExampleGc

/* * */

static int LclExampleOpen(
  lua_State *L)

  // ExampleHnd <- example.open(PosNum)

{ // LclExampleOpen
  int Val;
  ExamplePtrType ExamplePtr;

  // Stk: Val?
  Val = luaL_checkint(L, 1);
  // Stk: Val
  ExamplePtr = lua_newuserdata(L, sizeof(ExampleType));
  printf("Opening " CnExampleStr " (%d)\n", Val);
  // Stk: Val ExampleHnd
  ExamplePtr->Val = Val;
  ExamplePtr->Open = 1;
  luaL_getmetatable(L, CnExampleStr);
  // Stk: Val ExampleHnd metatable
  lua_setmetatable(L, -2);
  // Stk: Val ExampleHnd
  return 1;
} // LclExampleOpen

/* * */

int luaopen_ud_example(
  lua_State *L)

{ // luaopen_ud_example

  static const luaL_reg MetaMap[] = {
    {"close", LclExampleClose},
    {"get", LclExampleGet},
    {"set", LclExampleSet},
    {"__tostring", LclExampleStr},
    {"__gc", LclExampleGc},
    {NULL, NULL}
  }; // MetaMap

  static const luaL_reg Map[] = {
    {"open", LclExampleOpen},
    {NULL, NULL}
  }; // Map

  // Stk: ModuleStr
  // Create metatable for handles of type "example"
  luaL_newmetatable(L, CnExampleStr);
  // Stk: ModuleStr Meta
  // Push copy of metatable
  lua_pushvalue(L, -1);
  // Stk: ModuleStr Meta Meta
  // Retrieve indexed fields from metatable itself
  lua_setfield(L, -2, "__index");
  // Stk: ModuleStr Meta
  // Register functions in metatable at top of stack
  luaL_register(L, NULL, MetaMap);
  // Stk: ModuleStr Meta
  luaL_register(L, "ud_example", Map);
  // Stk: ModuleStr Meta Namespace
  return 1;
} // luaopen_ud_example
