extern "C" 
{
#include <lua-5.3.3/lua.h>
#include <lua-5.3.3/lauxlib.h>
#include <lua-5.3.3/lualib.h>
}


#include "LuaBridge/LuaBridge.h"

extern "C" __declspec(dllimport) int luaopen_shell(lua_State* L);

class A 
{
protected:
	std::string name;

public:
	A(std::string s)
	{
		this->name = s;
	}

	std::string getName()
	{
		return this->name;
	}

	void printName()
	{
		printf("Hello, my name is %s!\n", this->name.c_str());
	}
};

void report_errors(lua_State *L, int status)
{
	if (status != 0) 
	{
		printf("-- %s\n", lua_tostring(L, -1));
		lua_pop(L, 1); // remove error message
	}
}

int main()
{
	// create a Lua state
	lua_State* L = luaL_newstate();

	// load standard libs 
	luaL_openlibs(L);             
	luaopen_shell(L);

	luabridge::getGlobalNamespace(L)
		//.beginNamespace("test")
		.beginClass<A>("A")
		.addConstructor<void(*) (std::string)>()
		.addFunction("getName", &A::getName)
		.addFunction("printName", &A::printName)
		.endClass();
	//.endNamespace();

	A* a = new A("some_var");
	luabridge::push(L, a);
	lua_setglobal(L, "a");

	int lscript = luaL_dofile(L, "test02.lua");

	report_errors(L, lscript);

	luabridge::LuaRef runHandler = luabridge::getGlobal(L, "runHandler");

	int x = runHandler(15, 12);

	printf("(15 + 12) * 2 = %d\n", x);

	return 0;
}