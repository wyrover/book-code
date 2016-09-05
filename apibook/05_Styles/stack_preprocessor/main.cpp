/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  An example of a pre-processor based API.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "stack.h"
#include <iostream>

using std::cout;
using std::endl;

int main(int, char **)
{
	// create a new stack of integers
	IntStack stack;

	// push and pop a value to the stack
	cout << "Empty: " << stack.IsEmpty() << endl;
	stack.Push(10);
	cout << "Empty: " << stack.IsEmpty() << endl;
	int val = stack.Pop();
	cout << "Popped off: " << val << endl;
	cout << "Empty: " << stack.IsEmpty() << endl;

	return 0;
}
