/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   pluginmanager.cpp
/// \author Martin Reddy
/// \brief  A Plugin Manager singleton
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "pluginmanager.h"
#include <string>
#include <map>
#include <set>

#if _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

class PluginInstance::Impl
{
public:
	std::string mFileName;
	std::string mDisplayName;
#if _WIN32
	// Plugin loading routines for Windows
	HINSTANCE handle;

	typedef FARPROC PluginFunc;

	bool Load()
	{
		handle = LoadLibrary(mFileName.c_str());
		return (handle != NULL);
	}

	bool Unload()
	{
		if (!handle)
			return true;
		return FreeLibrary(handle) != 0;
	}

	PluginFunc GetFunction(const std::string &name)
	{
		PluginFunc fptr = GetProcAddress(handle, name.c_str());
		if (! fptr)
			return NULL;
		return fptr;
	}
#else
	// Plugin loading routines for Mac & Linux
	void *handle;

	typedef void(*PluginFunc)();

	bool Load()
	{
		handle = dlopen(mFileName.c_str(), RTLD_LOCAL | RTLD_LAZY);
		return (handle != NULL);
	}

	bool Unload()
	{
		if (!handle)
			return true;
		return dlclose(handle) == 0;
	}

	PluginFunc GetFunction(const std::string &name)
	{
		dlerror();
		PluginFunc fptr = (PluginFunc) dlsym(handle, name.c_str());
		if (dlerror())
			return NULL;
		return fptr;
	}
#endif


};

PluginInstance::PluginInstance(const std::string &name)
{
	mImpl = new Impl;
	mImpl->mDisplayName = name;

#if _WIN32
	mImpl->mFileName = name + ".dll";
#elif __APPLE__
	mImpl->mFileName = "lib" + name + ".dylib";
#else
	mImpl->mFileName = "lib" + name + ".so";
#endif
}

PluginInstance::~PluginInstance()
{
	delete mImpl;
}

bool PluginInstance::Load()
{
	if (! mImpl->Load())
		return false;

	Impl::PluginFunc init_func = mImpl->GetFunction("PluginInit");
	if (!init_func)
		return false;

	(*init_func)();

	Impl::PluginFunc name_string = mImpl->GetFunction("PluginDisplayName");
	if (name_string)
	{
		const char **ptr = (const char **) name_string;
		mImpl->mDisplayName = *ptr;
	}

	return true;
}

bool PluginInstance::Unload()
{
	return mImpl->Unload();
}

bool PluginInstance::IsLoaded()
{
	return (mImpl->handle != NULL);
}

std::string PluginInstance::GetFileName()
{
	return mImpl->mFileName;
}

std::string PluginInstance::GetDisplayName()
{
	return mImpl->mDisplayName;
}

PluginManager::PluginManager()
{
}

PluginManager::~PluginManager()
{
}

PluginManager &PluginManager::GetInstance()
{
	static PluginManager mgr;
	return mgr;
}

bool PluginManager::LoadAll()
{
	// hardcode loading our 2 example plugins
	// in reality, this would search for all plugins to load
	Load("plugin1");
	Load("plugin2");
	return true;
}

bool PluginManager::Load(const std::string &name)
{
	// have we already loaded this plugin?
	static std::set<std::string> loaded;
	if (loaded.find(name) != loaded.end())
		return true;

	// try to load this plugin
	PluginInstance *pi = new PluginInstance(name);
	if (! pi->Load())
	{
		delete pi;
		return false;
	}

	// success! add the plugin to our manager
	mPlugins.push_back(pi);

	// and don't try to load it again
	loaded.insert(name);
	return true;
}

bool PluginManager::UnloadAll()
{
	// not implemented yet
	return false;
}

bool PluginManager::Unload(const std::string &name)
{
	// not implemented yet
	return false;
}

std::vector<PluginInstance *> PluginManager::GetAllPlugins()
{
	// If we had an external metadata file that described all of the
	// plugins, this would be the point where we load that file -
	// without loading the actual plugins.  Without that external
	// metadata, we need to load all plugins.
	LoadAll();
	return mPlugins;
}
