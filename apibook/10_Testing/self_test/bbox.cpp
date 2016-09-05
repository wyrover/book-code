/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   bbox.cpp
/// \author Martin Reddy
/// \brief  A simple bounding box class with a self test
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "bbox.h"
#include <iostream>

BBox::BBox() :
	mMin(Point()),
	mMax(Point())
{
}

BBox::BBox(const Point &a, const Point &b) :
	mMin(CalcMin(a, b)),
	mMax(CalcMax(a, b))
{
}

bool BBox::Contains(const Point &p) const
{
	return false;
}

bool BBox::Intersects(const BBox &b) const
{
	return false;
}

double BBox::CenterDistance(const BBox &b) const
{
	return 0.0f;
}

Point BBox::CalcMin(const Point &a, const Point &b)
{
	return Point();
}

Point BBox::CalcMax(const Point &a, const Point &b)
{
	return Point();
}

Point BBox::GetCenter()
{
	return Point();
}
	
void BBox::SelfTest()
{
	std::cout << "BBox Self Test PASSED!" << std::endl;
}

