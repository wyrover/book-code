/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   myobject.cpp
/// \author Martin Reddy
/// \brief  A simple class to demonstrate shared pointers.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "myobject.h"
#include <iostream>

using std::cout;
using std::endl;

namespace apibook {

MyObject::MyObject(int id) :
	mID(id)
{
	cout << "... created object " << mID << endl;
}

MyObject::~MyObject()
{
	cout << "... destroyed object " << mID << endl;
}

MyObjectPtr MyObject::CreateInstance(int id)
{
	return MyObjectPtr(new MyObject(id));
}

}

