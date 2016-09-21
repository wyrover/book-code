#ifndef LUAARGS_H
#define LUAARGS_H


extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}


//------------------------------------------------------------------------------

struct Refs;

//------------------------------------------------------------------------------
class LuaArgs {
	lua_State *L;
	int tbl;
	int origin;
	Refs *refs;

private:
	LuaArgs();
	const char *scopy(int idx);
	const char *scopy(const char *src);
	const char *sref(int idx);
	const char *sref(const char *src);
	int fcopy(int idx);
	int fref(int idx);

public:
	LuaArgs(lua_State *ls);
	~LuaArgs();

	// Returns a count of the occupied stack slots
	// since this class instance was constructed.
	// It will throw an error if the count is less than 0.
	int count();

// Strings functions

	// Get a string by its (positive) index in the Lua stack.
	// Destructor will free the memory occupied this string.
	const char *gets(int idx, const char *Default);

	// Get a string either by its table key or by its table index.
	// Destructor will free the memory occupied this string.
	const char *gets(int idx, const char *key, const char *Default);

	// Get a string by its (positive) index in the Lua stack.
	// After using the string, the delete[] operator must be
	// called to free the memory occupied the string.
	const char *getS(int idx, const char *Default);

	// Get a string either by its table key or by its table index.
	// After using the string, the delete[] operator must be
	// called to free the memory occupied the string.
	const char *getS(int idx, const char *key, const char *Default);

// Integer functions

	// Get an integer value by its (positive) index in the Lua stack
	int geti(int idx, int Default);

	// Get an integer value either by its table key or by its table index.
	int geti(int idx, const char *key, int Default);

// Options functions

	// Get an option by its (positive) index in the Lua stack.
	// This function will return 0 if search fails, rather than throws an error.
	int getp(int idx, const char **list, const char *Default);

	// Get an option either by its table key or by its table index.
	// This function will return 0 if search fails, rather than throws an error.
	int getp(int idx, const char *key, const char **list, const char *Default);

// Floating point functions

	// Get a double value by its (positive) index in the Lua stack
	double getd(int idx, double Default);

	// Get a double value either by its table key or by its table index.
	double getd(int idx, const char *key, double Default);

// Boolean functions

	// Get a boolean value by its (positive) index in the Lua stack
	bool getb(int idx, bool Default);

	// Get a boolean value either by its table key or by its table index.
	bool getb(int idx, const char *key, bool Default);

// Function functions

	// Get a function unique index by its (positive) index in the Lua stack.
	// Destructor will release this index (returned value).
	int getf(int idx);

	// Get a function unique index either by its table key or by its table index.
	// Destructor will release this index (returned value).
	int getf(int idx, const char *key);

	// Get a function unique index by its (positive) index in the Lua stack.
	// After using the function, static releaseF() member function must be called
	// to release the function index (returned value). Alternatively, you can
	// directly invoke the luaL_unref() function for the LUA_REGISTRYINDEX table.
	int getF(int idx);

	// Get a function unique index either by its table key or by its table index.
	// After using the function, static releaseF() member function must be called
	// to release the function index (returned value). Alternatively, you can
	// directly invoke the luaL_unref() function for the LUA_REGISTRYINDEX table.
	int getF(int idx, const char *key);

	// Release an unique Lua registry index
	static void releaseF(lua_State *L, int ref);
};

#endif  // LUAARGS_H
