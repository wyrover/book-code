/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   pluginmanager.h
/// \author Martin Reddy
/// \brief  A Plugin Manager singleton
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include "defines.h"

#include <string>
#include <vector>

///
/// An object to represent a single plugin in the system
///
class CORE_API PluginInstance
{
public:
	explicit PluginInstance(const std::string &name);
	~PluginInstance();

	/// Load the plugin
	bool Load();
	/// Unload the plugin
	bool Unload();
	/// Return true if the plugin is loaded
	bool IsLoaded();

	/// Return the path to the plugin file on disk
	std::string GetFileName();
	/// Return the display name for the plugin
	std::string GetDisplayName();

private:
	// Make this object be noncopyable as it holds a pointer
	PluginInstance(const PluginInstance &);
	const PluginInstance &operator =(const PluginInstance &);

	class Impl;
	Impl *mImpl;
};

///
/// A manger for all plugins in the Core API
///
class CORE_API PluginManager
{
public:
	/// Return the single instance of the plugin manager
	static PluginManager &GetInstance();

	/// Load all plugins
	bool LoadAll();
	/// Load a single named plugin
	bool Load(const std::string &name);

	/// Unload all plugins
	bool UnloadAll();
	/// Unload a single named plugin
	bool Unload(const std::string &name);

	/// Return a list of all plugins
	std::vector<PluginInstance *> GetAllPlugins();

private:
	PluginManager();
	~PluginManager();
	PluginManager(const PluginManager &);
	const PluginManager &operator =(const PluginManager &);

	std::vector<PluginInstance *> mPlugins;
};


#endif
