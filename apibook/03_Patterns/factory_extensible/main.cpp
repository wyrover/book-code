/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  An example of an extensible Factory Method.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "rendererfactory.h"
#include <iostream>

using std::cout;
using std::endl;

/// An OpenGL-based 3D renderer
class OpenGLRenderer : public IRenderer
{
public:
	~OpenGLRenderer() {}
	bool LoadScene(const std::string &filename) { return true; }
	void SetViewportSize(int w, int h) {}
	void SetCameraPos(double x, double y, double z) {}
	void SetLookAt(double x, double y, double z) {}
	void Render() { cout << "OpenGL Render" << endl; }
	static IRenderer *Create() { return new OpenGLRenderer; }
};

/// A DirectX-based 3D renderer
class DirectXRenderer : public IRenderer
{
public:
	bool LoadScene(const std::string &filename) { return true; }
	void SetViewportSize(int w, int h) {}
	void SetCameraPos(double x, double y, double z) {}
	void SetLookAt(double x, double y, double z) {}
	void Render() { cout << "DirectX Render" << endl; }
	static IRenderer *Create() { return new DirectXRenderer; }
};

/// A Mesa-based software 3D renderer
class MesaRenderer : public IRenderer
{
public:
	bool LoadScene(const std::string &filename) { return true; }
	void SetViewportSize(int w, int h) {}
	void SetCameraPos(double x, double y, double z) {}
	void SetLookAt(double x, double y, double z) {}
	void Render() { cout << "Mesa Render" << endl; }
	static IRenderer *Create() { return new MesaRenderer; }
};


int main(int, char **)
{
	// register the various 3D renderers with the factory object
	RendererFactory::RegisterRenderer("opengl", OpenGLRenderer::Create);
	RendererFactory::RegisterRenderer("directx", DirectXRenderer::Create);
	RendererFactory::RegisterRenderer("mesa", MesaRenderer::Create);

	// create an OpenGL renderer
	IRenderer *ogl = RendererFactory::CreateRenderer("opengl");
	ogl->Render();
	delete ogl;

	// create a Mesa software renderer
	IRenderer *mesa = RendererFactory::CreateRenderer("mesa");
	mesa->Render();
	delete mesa;

	// unregister the Mesa renderer
	RendererFactory::UnregisterRenderer("mesa");
	mesa = RendererFactory::CreateRenderer("mesa");
	if (! mesa)
	{
		cout << "Mesa renderer unregistered" << endl;
	}

	return 0;
}
