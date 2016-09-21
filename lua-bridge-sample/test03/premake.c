#include "premake.h"

/**
 * Locate a file in the embedded script index. If found, returns the
 * contents of the file's script.
 */

 const buildin_mapping* premake_find_embedded_script(const char* filename)
 {
#if !defined(PREMAKE_NO_BUILTIN_SCRIPTS)
 	int i;
	for (i = 0; builtin_scripts[i].name != NULL; ++i) {
		if (strcmp(builtin_scripts[i].name, filename) == 0) {
			return builtin_scripts + i;
		}
	}
#endif
	return NULL;
 }


/**
 * Load a script that was previously embedded into the executable. If
 * successful, a function containing the new script chunk is pushed to
 * the stack, just like luaL_loadfile would do had the chunk been loaded
 * from a file.
 */

int premake_load_embedded_script(lua_State* L, const char* filename)
{
#if !defined(NDEBUG)
	static int warned = 0;
#endif

	const buildin_mapping* chunk = premake_find_embedded_script(filename);
	if (chunk == NULL) {
		return !OKAY;
	}

	/* Debug builds probably want to be loading scripts from the disk */
#if !defined(NDEBUG)
	if (!warned) {
		warned = 1;
		printf("** warning: using embedded script '%s'; use /scripts argument to load from files\n", filename);
	}
#endif

	/* "Fully qualify" the filename by turning it into the form $/filename */
	lua_pushstring(L, "$/");
	lua_pushstring(L, filename);
	lua_concat(L, 2);

	/* Load the chunk */
	return luaL_loadbuffer(L, (const char*)chunk->bytecode, chunk->length, filename);
}