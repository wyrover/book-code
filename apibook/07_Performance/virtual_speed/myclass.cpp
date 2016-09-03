/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   myclass.cpp
/// \author Martin Reddy
/// \brief  Classes with and without virtual function calls.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "myclass.h"

NonVirtualClass::NonVirtualClass() :
	mCounter(0)
{
}

NonVirtualClass::~NonVirtualClass()
{
}

void NonVirtualClass::DoSomething()
{
	mCounter++;
}

BaseClass::BaseClass() :
	mCounter(0)
{
}

BaseClass::~BaseClass()
{
}

void BaseClass::DoSomething()
{
}

void DerivedClass::DoSomething()
{
	mCounter++;
}
