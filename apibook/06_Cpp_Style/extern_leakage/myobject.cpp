/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   myobject.cpp
/// \author Martin Reddy
/// \brief  Showing how "internal" details can be leaked.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "myobject.h"
#include <iostream>
#include <string>

using std::cout;
using std::endl;

// Uncomment the next line to prevent external linkage leakage
// namespace {

// an integer constant with external linkage
int INTERNAL_CONSTANT = 42;

// a string constant with external linkage
std::string Filename = "file.txt";

// a non-static function with external linkage
void FreeFunction()
{
	cout << "Free function called" << endl;
}

// Uncomment the next line to prevent external linkage leakage
//}


namespace apibook {

void MyObject::SetValue(int value)
{
	mValue = value;
}

int MyObject::GetValue() const
{
	return mValue;
}

}
