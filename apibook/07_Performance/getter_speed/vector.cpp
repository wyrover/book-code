/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   vector.cpp
/// \author Martin Reddy
/// \brief  Classes with different data member accessors.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "vector.h"

ExposedVector::ExposedVector(double x, double y, double z) :
	mX(x), mY(y), mZ(z)
{
}

HiddenVector::HiddenVector(double x, double y, double z) :
	mX(x), mY(y), mZ(z)
{
}

InlinedVector::InlinedVector(double x, double y, double z) :
	mX(x), mY(y), mZ(z)
{
}

void HiddenVector::setX(double val) { mX = val; }
void HiddenVector::setY(double val) { mY = val; }
void HiddenVector::setZ(double val) { mZ = val; }
double HiddenVector::getX() const { return mX; }
double HiddenVector::getY() const { return mY; }
double HiddenVector::getZ() const { return mZ; }

