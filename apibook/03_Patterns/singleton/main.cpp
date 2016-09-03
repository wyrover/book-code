/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  An example of the Singleton design pattern.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "singleton.h"
#include <iostream>

using std::cout;
using std::endl;

int main(int, char **)
{
	cout << "main() called" << endl;

	// singleton gets created here
	Singleton &foo = Singleton::GetInstance();
	foo.GetState();

	// returns previously allocated singleton here
	Singleton &foo2 = Singleton::GetInstance();
	foo2.GetState();

	return 0;
}
