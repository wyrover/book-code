/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   original.cpp
/// \author Martin Reddy
/// \brief  An original class to be wrapped by a Facade.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "original.h"
#include <iostream>

using std::cout;
using std::endl;

int Original1::DoOperation1()
{
	cout << "In Original::DoOperation1" << endl;
	return 42;
}

bool Original2::DoOperation2(int value)
{
	cout << "In Original::DoOperation2 with value " << value << endl;
	return true;
}
