/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   myclass.h
/// \author Martin Reddy
/// \brief  Classes with and without virtual function calls.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef MYCLASS_H
#define MYCLASS_H

#include <stdio.h>

///
/// A class that has no virtual functions
///
class NonVirtualClass
{
public:
	NonVirtualClass();
	~NonVirtualClass();

	void DoSomething();

private:
	size_t mCounter;
};

///
/// A base class that has virtual functions
///
class BaseClass
{
public:
	BaseClass();
	virtual ~BaseClass();

	virtual void DoSomething();

protected:
	size_t mCounter;
};

///
/// A class that inherits from BaseClass and overrides a function
///
class DerivedClass : public BaseClass
{
public:
	/*virtual*/ void DoSomething();
};

#endif
