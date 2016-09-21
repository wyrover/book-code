#ifndef lua_engine_h__
#define lua_engine_h__

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <LuaBridge.h>

#include <string>

namespace cactus
{
class LuaEngine
{
public:
	const static std::string kDefaultScriptDir;

	explicit LuaEngine();
	~LuaEngine();
public:
	int init(bool loadStdLib = true);
	int registerFunction(lua_CFunction fn, std::string name);
	int executeString(const std::string lua_expression);
	int executeFile(const std::string file);
	bool loadScirpt(std::string filename);
	std::string loadScript(std::wstring filename);
	std::string scriptDirectory() const 
	{
		return script_directory_;
	}

	void setScriptDirectory(std::string directory)
	{
		script_directory_ = directory;
	}

	lua_State* getState();
	
	luabridge::LuaRef getGlobal(std::string name);

	void reset();
private:
	void report_errors(int state);
	
private:
	lua_State* state_;
	std::string script_directory_;
};
} // namespace cactus


#endif // lua_engine_h__
