/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   pluginapi.h
/// \author Martin Reddy
/// \brief  An API that lets users write plugins.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef PLUGINAPI_H
#define PLUGINAPI_H

#include "defines.h"
#include "renderer.h"

#define PLUGIN_API_VERSION  1

#define CORE_FUNC     extern "C" CORE_API
#define PLUGIN_FUNC   extern "C" PLUGIN_API

/// declare the initialization routine for a plugin
#define PLUGIN_INIT() \
	const int PluginVersion = PLUGIN_API_VERSION; \
	PLUGIN_FUNC int PluginInit()

/// declare the cleanup routine for a plugin
#define PLUGIN_FREE() \
	PLUGIN_FUNC int PluginFree()

/// declare the display name a plugin
#define PLUGIN_DISPLAY_NAME(name) \
	PLUGIN_API const char * PluginDisplayName = name

/// declare the minimum required Plugin API version for a plugin
#define PLUGIN_MIN_VERSION(version) \
	PLUGIN_API const char * PluginMinVersion = version

/// declare the maximum supported Plugin API version for a plugin
#define PLUGIN_MAX_VERSION(version) \
	PLUGIN_API const char * PluginMaxVersion = version


/// The function signature for a routine that creates a renderer
typedef IRenderer *(*RendererInitFunc)();
/// The function signature for a routine that destroys a renderer
typedef void (*RendererFreeFunc)(IRenderer *);

/// A routine to let a plugin register a new renderer type
CORE_FUNC void RegisterRenderer(const char *type,
								RendererInitFunc init_cb,
								RendererFreeFunc free_cb);


#endif
