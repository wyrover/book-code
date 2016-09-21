/**
 * \file   lua_auxlib.c
 * \brief  Modifications and extensions to Lua's library functions.
 * \author Copyright (c) 2014-2015 Jason Perkins and the Premake project
 */

#include "premake.h"
#include <Windows.h>
#include <string>

static int chunk_wrapper(lua_State* L);



/* Pull in Lua's aux lib implementation, but rename luaL_loadfile() so I
 * can replace it with my own implementation. */

#define luaL_loadfilex original_luaL_loadfile
#include "lauxlib.c"
#undef luaL_loadfilex

std::string StringReplace(std::string str, const std::string &strReplace, const std::string &strWith, unsigned int count = 0)
{
	size_t pos = str.find(strReplace);

	for (unsigned int i = 0; pos != std::string::npos && (i < count || count == 0); i++) {
		str.replace(pos, strReplace.size(), strWith);
		pos = str.find(strReplace);
	}

	return str;
}



void test1()
{
	::MessageBoxA(NULL, "test", "test", MB_OK);
}

const char* GetAllPathDelimiters()
{
	return "\\/";
}

std::string ExtractFilePath(const std::string& FileName)
{
	std::string Result = FileName;
	return Result.erase(Result.find_last_of(GetAllPathDelimiters()) + 1);
}

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

std::string GetAppFileName()
{
	char filename[MAX_PATH + 10] = {0};
	GetModuleFileNameA((HMODULE)&__ImageBase, filename, MAX_PATH);
	return std::string(filename);
}

std::string GetAppDir()
{
	return ExtractFilePath(GetAppFileName());
}

/**
 * Extend the default implementation of luaL_loadfile() to call my chunk
 * wrapper, above, before executing any scripts loaded from a file.
 */

LUALIB_API int luaL_loadfilex (lua_State* L, const char* filename, const char *mode)
{

	//::MessageBoxA(NULL, GetAppDir().c_str(), "Ô­Ê¼Â·¾¶", MB_OK);
	char fullpath[PATH_MAX + 1] = {0};
	GetFullPathNameA(filename, PATH_MAX, fullpath, NULL);
	std::string full_filename = StringReplace(std::string(fullpath), "/", "\\");
	//::MessageBoxA(NULL, full_filename.c_str(), "full", MB_OK);
	
	std::string name1 = StringReplace(full_filename, GetAppDir(), "");
	name1 = StringReplace(name1, "\\", "/");

	//::MessageBoxA(NULL, name1.c_str(), "test", MB_OK);


	

	

	const char* script_dir;
	const char* test_name;

	
	int z = !OKAY;

	

	/* If filename is starts with "$/" then we want to load the version that
	 * was embedded into the executable and skip the local file system */
	if (filename[0] == '$') {
		z = premake_load_embedded_script(L, filename + 2); /* Skip over leading "$/" */
		if (z != OKAY) return z;
	}

	/* If the currently running script was embedded, try to load this file
	 * as it if were embedded too. */
	/* Try to locate the script on the filesystem */
	if (z != OKAY) {
		//lua_pushcfunction(L, os_locate);
		//lua_pushstring(L, filename);
		//lua_call(L, 1, 1);

		//test_name = lua_tostring(L, -1);
		test_name = filename;
		if (test_name) {
			z = original_luaL_loadfile(L, test_name, mode);
		}

		/* If the file exists but errors, pass that through */
		if (test_name && z != OKAY && z != LUA_ERRFILE) {
			return z;
		}

		/* If the file didn't exist, remove the result and the test
		 * name from the stack before checking embedded scripts */
		if (z != OKAY) {
			lua_pop(L, 1);
		}
	}

	/* Try to load from embedded scripts */
	if (z != OKAY) {
		z = premake_load_embedded_script(L, name1.c_str());
		::MessageBoxA(NULL, name1.c_str(), "test", MB_OK);
	}

	/* Either way I should have ended up with the file name followed by the
	 * script chunk on the stack. Turn these into a closure that will call my
	 * wrapper below when the loaded script needs to be executed. */
	if (z == OKAY) {
		//lua_pushcclosure(L, chunk_wrapper, 2);
	}
	else if (z == LUA_YIELD) {
		lua_pushstring(L, "cannot open ");
		lua_pushstring(L, filename);
		lua_pushstring(L, ": No such file or directory");
		lua_concat(L, 3);
	}

	return z;
}



/**
 * Execute a chunk of code previously loaded by my customized version of
 * luaL_loadfile(), below. Sets the _SCRIPT global variable to the absolute
 * path of the loaded chunk, and makes its enclosing directory current so
 * that relative path references to other files or scripts can be used.
 */

static int chunk_wrapper(lua_State* L)
{
	char cwd[PATH_MAX];
	const char* filename;
	char* ptr;
	int i, args;

	args = lua_gettop(L);

	/* Remember the current _SCRIPT and working directory so I can
	 * restore them after this new chunk has been run. */

	do_getcwd(cwd, PATH_MAX);
	lua_getglobal(L, "_SCRIPT");
	lua_getglobal(L, "_SCRIPT_DIR");

	/* Set the new _SCRIPT variable */

	lua_pushvalue(L, lua_upvalueindex(1));
	lua_setglobal(L, "_SCRIPT");

	/* And the new _SCRIPT_DIR variable (const cheating) */

	filename = lua_tostring(L, lua_upvalueindex(1));
	ptr = (char*)strrchr(filename, '/');
	if (ptr) *ptr = '\0';
	lua_pushstring(L, filename);
	lua_setglobal(L, "_SCRIPT_DIR");

	/* And make that the CWD (and fix the const cheat) */

	if (filename[0] != '$') {
		do_chdir(L, filename);
	}
	if (ptr) *ptr = '/';

	/* Move the function's arguments to the top of the stack and
	 * execute the function created by luaL_loadfile() */

	lua_pushvalue(L, lua_upvalueindex(2));
	for (i = 1; i <= args; ++i) {
		lua_pushvalue(L, i);
	}

	lua_call(L, args, LUA_MULTRET);

	/* Finally, restore the previous _SCRIPT variable and working directory
	 * before returning control to the previously executing script. */

	do_chdir(L, cwd);
	lua_pushvalue(L, args + 1);
	lua_setglobal(L, "_SCRIPT");
	lua_pushvalue(L, args + 2);
	lua_setglobal(L, "_SCRIPT_DIR");

	return lua_gettop(L) - args - 2;
}
