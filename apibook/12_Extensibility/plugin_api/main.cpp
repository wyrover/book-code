/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  Shows how to create a plugin API.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "coreapi.h"
#include "pluginmanager.h"
#include <iostream>

///
/// A Mesa-based software renderer
///
class MesaRenderer : public IRenderer
{
public:
	bool LoadScene(const char *filename) { return true; }
	void SetViewportSize(int w, int h) {}
	void SetCameraPos(double x, double y, double z) {}
	void SetLookAt(double x, double y, double z) {}
	void Render() { std::cout << "Mesa Render" << std::endl; }
	static IRenderer *Create() { return new MesaRenderer; }
};

int main(int, char **)
{
	// create the factory object
	RendererFactory::RegisterRenderer("mesa", MesaRenderer::Create);

	// print the pretty name of all available plugins
	std::vector<PluginInstance *> plugins = PluginManager::GetInstance().GetAllPlugins();
	std::vector<PluginInstance *>::iterator it;
	for (it = plugins.begin(); it != plugins.end(); ++it)
	{
		PluginInstance *pi = *it;
		std::cout << "Plugin: " << pi->GetDisplayName() << std::endl;
	}
	

	// load all plugins
	PluginManager::GetInstance().LoadAll();

	// create a Mesa software renderer (added by this module)
	IRenderer *mesa = RendererFactory::CreateRenderer("mesa");
	if (! mesa)
	{
		std::cout << "Failed to register 'mesa' renderer!" << std::endl;
		return 1;
	}

	mesa->Render();
	delete mesa;

	// create an OpenGL renderer (added by a plugin)
	IRenderer *ogl = RendererFactory::CreateRenderer("opengl");
	if (! ogl)
	{
		std::cout << "Failed to load 'opengl' plugin!" << std::endl;
		return 1;
	}

	ogl->Render();
	delete ogl;

	return 0;
}
