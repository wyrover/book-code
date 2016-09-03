/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  An example of using constructors and assignment.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "array.h"

using namespace apibook;

int main(int, char **)
{
	Array array1(3);        // default constructor called

	array1.Set(0, "Hello");
	array1.Print();

	Array array2;
	array2 = array1;        // assignment operator called
	array2.Set(1, "There");
	array2.Print();

	Array array3 = array1;  // copy constructor called
	array3.Print();

	return 0;
}
