/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   pluginapi.cpp
/// \author Martin Reddy
/// \brief  An API that lets users write plugins.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "coreapi.h"
#include "pluginapi.h"

void RegisterRenderer(const char *type,
					  RendererInitFunc init_cb,
					  RendererFreeFunc free_cb)
{
	RendererFactory::RegisterRenderer(type, init_cb);
	(void) free_cb; // we never free these objects
}

