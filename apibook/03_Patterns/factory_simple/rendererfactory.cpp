/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   rendererfactory.cpp
/// \author Martin Reddy
/// \brief  A factory object to create Renderer instances.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "rendererfactory.h"
#include <iostream>

using std::cout;
using std::endl;

class OpenGLRenderer : public IRenderer
{
public:
	~OpenGLRenderer() {}
	bool LoadScene(const std::string &filename) { return true; }
	void SetViewportSize(int w, int h) {}
	void SetCameraPos(double x, double y, double z) {}
	void SetLookAt(double x, double y, double z) {}
	void Render() { cout << "OpenGL Render" << endl; }
};

class DirectXRenderer : public IRenderer
{
public:
	bool LoadScene(const std::string &filename) { return true; }
	void SetViewportSize(int w, int h) {}
	void SetCameraPos(double x, double y, double z) {}
	void SetLookAt(double x, double y, double z) {}
	void Render() { cout << "DirectX Render" << endl; }
};

class MesaRenderer : public IRenderer
{
public:
	bool LoadScene(const std::string &filename) { return true; }
	void SetViewportSize(int w, int h) {}
	void SetCameraPos(double x, double y, double z) {}
	void SetLookAt(double x, double y, double z) {}
	void Render() { cout << "Mesa Render" << endl; }
};

IRenderer *RendererFactory::CreateRenderer(const std::string &type)
{
	if (type == "opengl")
		return new OpenGLRenderer;

	if (type == "directx")
		return new DirectXRenderer;

	if (type == "mesa")
		return new MesaRenderer;

	return NULL;
}
