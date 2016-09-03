/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   adapter.cpp
/// \author Martin Reddy
/// \brief  An example of the Adapter design pattern.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "adapter.h"
#include "original.h"
#include <iostream>

using std::cout;
using std::endl;

Adapter::Adapter()
	: mOrig(new Original)
{
	cout << "Allocated new Original object inside Adapter" << endl;
}

Adapter::~Adapter()
{
	delete mOrig;
	cout << "Destroyed Original object inside Adapter" << endl;
}

bool Adapter::DoSomething(int value)
{
	cout << "About to call Original::DoOperation from Adapter::DoSomething" << endl;
	mOrig->DoOperation(value, true);
	return true;
}

