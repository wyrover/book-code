/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   rendererfactory.h
/// \author Martin Reddy
/// \brief  A factory object to create IRenderer instances.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef RENDERERFACTORY_H
#define RENDERERFACTORY_H

#include "renderer.h"
#include <string>
#include <map>

///
/// A factory object that creates instances of different
/// 3D renderers. New renderers can be dynamically added
/// and removed from the factory object.
///
class RendererFactory
{
public:
	/// The type for the callback that creates an IRenderer instance
	typedef IRenderer *(*CreateCallback)();

	/// Add a new 3D renderer to the system
	static void RegisterRenderer(const std::string &type,
								 CreateCallback cb);
	/// Remove an existing 3D renderer from the system
	static void UnregisterRenderer(const std::string &type);

	/// Create an instance of a named 3D renderer
	static IRenderer *CreateRenderer(const std::string &type);

private:
	typedef std::map<std::string, CreateCallback> CallbackMap;
	static CallbackMap mRenderers;
};

#endif
