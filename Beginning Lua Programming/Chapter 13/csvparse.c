#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include <string.h>

/* * */

static const char * LclCsv(
  const char * Str,
  luaL_Buffer * BufPtr)

  /* This function parses the comma separated value (CSV) segment beginning at
  the start of Str. In CSV format

    * commas delimit text segments

    * double quotes which surround text are not copied

    * commas occurring within double quoted text are copied verbatim

    * each occurrence of two consecutive double quotes within double quoted
      text are copied as one double quote

  Examples

    abc               -> |abc|
    "abc"             -> |abc|
    "abc, def"        -> |abc, def|
    "abc ""def"" ghi" -> |abc "def" ghi|

  */

{ // LclCsv

  typedef enum {CnaIgnore = 0, CnaCopy = 1, CnaInc = 2, CnaQuit = 4} ActionType;

  typedef enum {CnsStart, CnsText, CnsQuoted, CnsHyperQuoted} StateType;

  typedef enum {CncComma, CncQuote, CncChar, CncNull} CatType;

  typedef struct {
    ActionType A;
    StateType S;
  } ContextType;

  static ContextType ContextList[CnsHyperQuoted + 1][CncNull + 1] = {

    { // CnsStart
      {CnaInc, CnsStart}, // CncComma
      {CnaIgnore, CnsQuoted}, // CncQuote
      {CnaCopy, CnsText}, // CncChar
      {CnaQuit, CnsStart}}, // CncNull

    { // CnsText
      {CnaInc, CnsStart}, // CncComma
      {CnaIgnore, CnsQuoted}, // CncQuote
      {CnaCopy, CnsText}, // CncChar
      {CnaInc | CnaQuit, CnsText}}, // CncNull

    { // CnsQuoted
      {CnaCopy, CnsQuoted}, // CncComma
      {CnaIgnore, CnsHyperQuoted}, // CncQuote
      {CnaCopy, CnsQuoted}, // CncChar
      {CnaInc | CnaQuit, CnsQuoted}}, // CncNull

    { // CnsHyperQuoted
      {CnaInc, CnsStart}, // CncComma
      {CnaCopy, CnsQuoted}, // CncQuote
      {CnaCopy, CnsText}, // CncChar
      {CnaInc | CnaQuit, CnsHyperQuoted}}}; // CncNull

  char Ch;
  ContextType Context;
  CatType Cat;

  Context.S = CnsStart;
  do {
    Ch = *(Str++);
    if (! Ch) Cat = CncNull;
    else if (Ch == 34) Cat = CncQuote;
    else if (Ch == ',') Cat = CncComma;
    else {
      Cat = CncChar;
      if (Ch < ' ') Ch = ' ';
    } // else
    Context = ContextList[Context.S][Cat];
    if (CnaCopy & Context.A) luaL_addchar(BufPtr, Ch);
    if (CnaInc & Context.A) Ch = 0;
  } while (Ch);
  return Str;
} // LclCsv

/* * */

static int LclCsvParse(
  lua_State *L)

  // str_segment, pos <- csv.parse(str [, pos])

{ // LclCsvParse
  const char *Str, *EndStr;
  int Len, Pos;
  luaL_Buffer Buf;

  Str = luaL_checkstring(L, 1);
  if (lua_isnil(L, 2)) Pos = 1;
  else Pos = luaL_checkinteger(L, 2);
  Len = strlen(Str);
  if ((Pos >= 1) && (Pos <= Len)) {
    luaL_buffinit(L, &Buf);
    EndStr = LclCsv(Str + Pos - 1, &Buf);
    luaL_pushresult(&Buf);
    Pos = EndStr - Str;
    Pos = Pos > Len ? -1 : Pos + 1;
    lua_pushinteger(L, Pos);
  } // if
  else luaL_error(L, "pos is out of range");
  return 2;
} // LclCsvParse

/* * */

int luaopen_csvparse(
  lua_State *L)

{ // luaopen_csvparse
  static const luaL_reg Map[] = {
    {"parse",  LclCsvParse},
    {NULL, NULL}
  };

  luaL_register(L, "csv", Map);
  return 1;
} // luaopen_csvparse
