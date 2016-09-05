/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   fireworks.cpp
/// \author Martin Reddy
/// \brief  An illustration of using private classes.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "fireworks.h"
#include <iostream>

using std::cout;
using std::endl;

namespace apibook {

Fireworks::Fireworks() :
	mOriginX(0.0),
	mOriginY(0.0),
	mRed(0.0),
	mGreen(0.0),
	mBlue(0.0),
	mGravity(0.0),
	mSpeed(0.0),
	mIsActive(false)
{
}

void Fireworks::SetOrigin(double x, double y)
{
	mOriginX = x;
	mOriginY = y;
}

void Fireworks::SetColor(float r, float g, float b)
{
	mRed   = r;
	mGreen = g;
	mBlue  = b;
}

void Fireworks::SetGravity(float g)
{
	mGravity = g;
}

void Fireworks::SetSpeed(float s)
{
	mSpeed = s;
}

void Fireworks::SetNumberOfParticles(int num)
{
}

void Fireworks::Start()
{
	cout << "Fireworks started!" << endl;
}

void Fireworks::Stop()
{
	cout << "Fireworks stopped!\n" << endl;
}

void Fireworks::NextFrame(float dt)
{
	cout << "Next Frame..." << endl;
}

}
