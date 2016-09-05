/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   coreapi.h
/// \author Martin Reddy
/// \brief  A factory object to create Renderer instances.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef COREAPI_H
#define COREAPI_H

#include "defines.h"
#include "renderer.h"

#include <string>
#include <map>

///
/// A factory object in the Core API
///
class CORE_API RendererFactory
{
public:
	typedef IRenderer *(*CreateCallback)();

	static void RegisterRenderer(const std::string &type,
								 CreateCallback cb);
	static void UnregisterRenderer(const std::string &type);

	static IRenderer *CreateRenderer(const std::string &type);

private:
	typedef std::map<std::string, CreateCallback> CallbackMap;
	static CallbackMap mRenderers;
};

#endif
