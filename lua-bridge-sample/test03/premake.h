#ifndef premake_h__
#define premake_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

	/* Fill in any missing bits */
#ifndef PATH_MAX
#define PATH_MAX   (4096)
#endif

#define OKAY   (0)

	/* Engine interface */

	typedef struct
	{
		const char*          name;
		const unsigned char* bytecode;
		size_t               length;
	} buildin_mapping;

	extern const buildin_mapping builtin_scripts[];


	/* Bootstrapping helper functions */
	int do_chdir(lua_State* L, const char* path);
	int do_getcwd(char* buffer, size_t size);
	int do_isfile(const char* filename);
	int do_isabsolute(const char* path);
	int do_pathsearch(lua_State* L, const char* filename, const char* path);
	void do_translate(char* value, const char sep);

	int os_locate(lua_State* L);
	int path_getabsolute(lua_State* L);

int premake_load_embedded_script(lua_State* L, const char* filename);

#ifdef __cplusplus
}
#endif



#endif // premake_h__
