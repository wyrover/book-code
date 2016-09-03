/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   plugin1.h
/// \author Martin Reddy
/// \brief  An example plugin built against the plugin API.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "pluginapi.h"
#include <iostream>

class OpenGLRenderer : public IRenderer
{
public:
	~OpenGLRenderer() {}
	bool LoadScene(const char *filename) { return true; }
	void SetViewportSize(int w, int h) {}
	void SetCameraPos(double x, double y, double z) {}
	void SetLookAt(double x, double y, double z) {}
	void Render() { std::cout << "OpenGL Render" << std::endl; }
};

PLUGIN_FUNC IRenderer *CreateRenderer()
{
	return new OpenGLRenderer;
}

PLUGIN_FUNC void DestroyRenderer(IRenderer *r)
{
	delete r;
}

PLUGIN_DISPLAY_NAME("OpenGL Renderer");

PLUGIN_INIT()
{
	// register our new renderer
	std::cout << "PLUGIN_INIT" << std::endl;
	RegisterRenderer("opengl", CreateRenderer, DestroyRenderer);
	return 0;
}
