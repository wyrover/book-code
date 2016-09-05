/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   ioriginal.h
/// \author Martin Reddy
/// \brief  A common interface for Proxy and Original.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef IORIGINAL_H
#define IORIGINAL_H

///
/// A shared abstract interface for Proxy and Original.
///
class IOriginal
{
public:
	virtual ~IOriginal() {};
	virtual bool DoSomething(int value) = 0;
};

#endif
