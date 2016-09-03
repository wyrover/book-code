/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  Show pass by value and reference overhead.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "myobject.h"
#include <iostream>

using namespace apibook;
using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
	cout << "*** Create object" << endl;

	MyObjectHolder holder;
	MyObject object;

	cout << "*** Pass by reference" << endl;

	holder.SetObjectByConstReference(object);

	cout << "*** Pass by value" << endl;

	holder.SetObjectByValue(object);

	cout << "*** exit()" << endl;
	return 0;
}
