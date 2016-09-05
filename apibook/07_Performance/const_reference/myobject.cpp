/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   myobject.h
/// \author Martin Reddy
/// \brief  Show differences between pass by value/reference
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

MySubObject::MySubObject()
{
	cout << "MySubObject Constructor" << endl;
}

MySubObject::~MySubObject()
{
	cout << "MySubObject Destructor" << endl;
}

MySubObject::MySubObject(const MySubObject &obj)
{
	cout << "MySubObject Copy Constructor" << endl;
}

MySubObject &MySubObject::operator = (const MySubObject &obj)
{
	cout << "MySubObject Assignment operator" << endl;
	return *this;
}

MyObject::MyObject()
{
	cout << "MyObject Constructor" << endl;
}

MyObject::~MyObject()
{
	cout << "MyObject Destructor" << endl;
}

MyObject::MyObject(const MyObject &obj)
{
	cout << "MyObject Copy Constructor" << endl;
}

MyObject &MyObject::operator = (const MyObject &obj)
{
	cout << "MyObject Assignment operator" << endl;
	return *this;
}

MyObjectHolder::MyObjectHolder()
{
	cout << "MyObjectHolder Constructor" << endl;
}

MyObjectHolder::~MyObjectHolder()
{
	cout << "MyObjectHolder Destructor" << endl;
}

void MyObjectHolder::SetObjectByValue(MyObject obj)
{
	cout << "MyObjectHolder::SetObjectByValue()" << endl;
	mObject = obj;
}

void MyObjectHolder::SetObjectByConstReference(const MyObject &obj)
{
	cout << "MyObjectHolder::SetObjectByConstReference()" << endl;
	mObject = obj;
}

}
