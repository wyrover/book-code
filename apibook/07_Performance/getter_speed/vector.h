/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   vector.h
/// \author Martin Reddy
/// \brief  Classes with different data member accessors.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef VECTOR_H
#define VECTOR_H

///
/// A class where data members are exposed as public
///
class ExposedVector
{
public:
	ExposedVector(double x, double y, double z);
	double mX, mY, mZ;
};

///
/// A class where data members are accessed via getter/setter functions
///
class HiddenVector
{
public:
	HiddenVector(double x, double y, double z);
	void setX(double val);
	void setY(double val);
	void setZ(double val);
	double getX() const;
	double getY() const;
	double getZ() const;

private:
	double mX, mY, mZ;
};

///
/// A class where data members are accessed via inlined getter/setter functions
///
class InlinedVector
{
public:
	InlinedVector(double x, double y, double z);
	void setX(double val);
	void setY(double val);
	void setZ(double val);
	double getX() const;
	double getY() const;
	double getZ() const;

private:
	double mX, mY, mZ;
};

// define the inlined member functions outside of the class definition.
// normally you would put these in a separate header - they're here for
// clarity for this example.
inline void InlinedVector::setX(double val) { mX = val; }
inline void InlinedVector::setY(double val) { mY = val; }
inline void InlinedVector::setZ(double val) { mZ = val; }
inline double InlinedVector::getX() const { return mX; }
inline double InlinedVector::getY() const { return mY; }
inline double InlinedVector::getZ() const { return mZ; }

#endif
