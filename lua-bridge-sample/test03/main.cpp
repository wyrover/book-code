
#include "lua_engine.h"
#include "thread_pool.h"
#include <iostream>

#include <boost/thread/thread.hpp>

typedef signed __int64       int64_t;
typedef unsigned __int64     uint64_t;

using namespace cactus;

class Cleaner
{
public:
	void do_job(std::string str)
    {
        //std::cout << "Hello world, I'm a thread!" << std::endl;
        LuaEngine engine;

        if (engine.init()) {
            lua_State* L = engine.getState();
            luabridge::getGlobalNamespace(L)
				.beginClass<Cleaner>("Cleaner")
					.addConstructor<void(*)(void)>()
					//.addFunction("hello", &Cleaner::hello)
					.addFunction("print", &Cleaner::print)
					.addFunction("printFileSize", &Cleaner::printFileSize)
				.endClass();
            luabridge::push(L, this);
            lua_setglobal(L, "cleaner");
            engine.loadScirpt("test.lua");
            //luabridge::LuaRef filesizeRef = engine.getGlobal("filesize");
            //long long filesize = filesizeRef.cast<long long>();
            //std::cout << "read lua variable from C++ code:" << filesize << std::endl;
        }
    }

    void print(std::string message)
    {
        std::cout << message << std::endl;
    }

    void printFileSize(long long filesize)
    {
        std::cout << "文件大小:" << filesize << std::endl;
    }

	

};

int main()
{
	{
		thread_pool tp(10);
		Cleaner cleaner;

		for (int i = 1; i <= 10000; ++i) {
			char data[256] = {0};

			std::string message = "test";
			message.append(itoa(i, data, 10));
			tp.enqueue(boost::bind(&Cleaner::do_job, &cleaner, message));			
		}
	}

    return 0;
}