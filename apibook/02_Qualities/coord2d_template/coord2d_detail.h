/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   coord2d.h
/// \author Martin Reddy
/// \brief  An example templated 2D coordinate class.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef COORD2D_DETAIL_H
#define COORD2D_DETAIL_H

// define the implementation for the Coord2D class template

namespace apibook {

template <class T>
Coord2D<T>::Coord2D(T x, T y) : mX(x), mY(y)
{
};

template <class T>
T Coord2D<T>::GetX() const
{
	return mX;
}

template <class T>
T Coord2D<T>::GetY() const
{
	return mY;
}

template <class T>
void Coord2D<T>::SetX(T x)
{
	mX = x;
}

template <class T>
void Coord2D<T>::SetY(T y)
{
	mY = y;
}

template <class T>
void Coord2D<T>::Add(T dx, T dy)
{
	mX += dx;
	mY += dy;
}

template <class T>
void Coord2D<T>::Multiply(T dx, T dy)
{
	mX *= dx;
	mY *= dy;
}


}

#endif
