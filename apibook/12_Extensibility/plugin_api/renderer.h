/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   renderer.h
/// \author Martin Reddy
/// \brief  An abstract base class for a simple 3D renderer.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef RENDERER_H
#define RENDERER_H

#include <string>

///
/// An abstract interface for plugins to create new renderers.
///
class IRenderer
{
public:
	virtual ~IRenderer() {}
	virtual bool LoadScene(const char *filename) = 0;
	virtual void SetViewportSize(int w, int h) = 0;
	virtual void SetCameraPos(double x, double y, double z) = 0;
	virtual void SetLookAt(double x, double y, double z) = 0;
	virtual void Render() = 0;
};

#endif
