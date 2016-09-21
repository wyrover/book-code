#include "lua_engine.h"
#include "lfs.h"
#include "winapi.h"
#include "lua_cjson.h"
#include <fstream>
#include <vector>


#include <tracetool/tracetool.h>



namespace cactus
{

const std::string LuaEngine::kDefaultScriptDir = "scripts/";

LuaEngine::LuaEngine() : state_(NULL), script_directory_(kDefaultScriptDir)
{
}

LuaEngine::~LuaEngine()
{
    if (state_) {
        lua_close(state_);
		state_ = NULL;
    }
}

int LuaEngine::init(bool loadStdLib)
{
 
	TRACE_ENTER_METHOD;
	state_ = luaL_newstate();

    if (loadStdLib)
        luaL_openlibs(state_);

	luaopen_lfs(state_);
    luaopen_winapi(state_);
	luaopen_cjson(state_);
	luaopen_cjson_safe(state_);

    /*luaopen_Game(state_);
    luaopen_BatchedRenderCube(state_);
    luaopen_Block(state_);
    luaopen_BlockRegistry(state_);
    luaopen_Dimension(state_);
    luaopen_Script(state_);
    luaopen_Player(state_);*/
	TRACE_EXIT_METHOD;
    return 1;
}

int LuaEngine::registerFunction(lua_CFunction fn, std::string name)
{
    // push the function (or static method) to lua
    lua_pushcfunction(state_, fn);
    // bind it to the given name
    lua_setglobal(state_, name.c_str());
    return 1;
}

int LuaEngine::executeString(const std::string lua_expression)
{
    int ret = luaL_dostring(state_, lua_expression.c_str());

	if (ret) {
		printf("%s\n", lua_tostring(state_, -1));
		lua_pop(state_, 1);
	}

    return ret == LUA_OK;
}

int LuaEngine::executeFile(const std::string file)
{
    const std::string filepath = file;
    int ret = luaL_dofile(state_, filepath.c_str());
	if (ret) {
		printf("%s\n", lua_tostring(state_, -1));
		lua_pop(state_, 1);
	}

    return ret == LUA_OK;
}

bool LuaEngine::loadScirpt(std::string filename)
{
   /* if (LUA_OK != luaL_loadfile(state_, filename.c_str())) {
        printf("lua: init failed. %s\n", lua_tostring(state_, -1));
        lua_pop(state_, 1);
        return false;
    }

    return lua_pcall(state_, 0, 0, 0) == LUA_OK;*/

	return luaL_dofile(state_, filename.c_str()) == LUA_OK;
}

std::string LuaEngine::loadScript(std::wstring filename)
{
    using namespace std;
    ifstream ifs(filename.c_str(), ios::in | ios::binary | ios::ate);
    ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg(0, ios::beg);
    vector<char> bytes(fileSize);
    ifs.read(&bytes[0], fileSize);
    return string(&bytes[0], fileSize);
}

lua_State* LuaEngine::getState()
{
	return state_;
}

luabridge::LuaRef LuaEngine::getGlobal(std::string name)
{
	return luabridge::getGlobal(state_, name.c_str());
}

void LuaEngine::reset()
{
	if (state_) {
		lua_close(state_);
		this->init(true);		
	}
}

void LuaEngine::report_errors( int state )
{
	if (state != 0) {
		printf("ERR: %s\n", lua_tostring(state_, state));
		lua_pop(state_, 1);
	}
}

}


