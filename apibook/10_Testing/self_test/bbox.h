/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   bbox.h
/// \author Martin Reddy
/// \brief  A simple bounding box class with a self test
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef BBOX_H
#define BBOX_H

struct Point
{
	double x, y, z;
};

///
/// A bounding box class
///
class BBox
{
public:
	BBox();
	BBox(const Point &a, const Point &b);

	/// Return the lower-left point
	Point GetMin() const;
	/// Return the upper-right point
	Point GetMax() const;

	/// Return true if point lies inside of this bbox
	bool Contains(const Point &point) const;
	/// Return true if bbox intersects with this bbox
	bool Intersects(const BBox &bbox) const;
	/// Return the distance bbox's center and this center
	double CenterDistance(const BBox &bbox) const;

	/// \cond TestFunctions
	void SelfTest();
	/// \endcond

private:
	Point CalcMin(const Point &a, const Point &b);
	Point CalcMax(const Point &a, const Point &b);
	Point GetCenter();
	
	Point mMin, mMax;
};

#endif
