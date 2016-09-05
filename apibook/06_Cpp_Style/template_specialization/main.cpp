/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  Template specialization of a class template.
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
	cout << "IntStack:" << endl;
	IntStack stack;

	cout << "Empty: " << stack.IsEmpty() << endl;
	stack.Push(10);
	cout << "Empty: " << stack.IsEmpty() << endl;
	int val = stack.Pop();
	cout << "Popped off: " << val << endl;
	cout << "Empty: " << stack.IsEmpty() << endl;

	cout << "IntPtrStack:" << endl;
	IntPtrStack stack2;
	int value = 42;

	cout << "Empty: " << stack2.IsEmpty() << endl;
	stack2.Push(&value);
	cout << "Empty: " << stack2.IsEmpty() << endl;
	int *valptr = stack2.Pop();
	cout << "Popped off: " << *valptr << endl;
	cout << "Empty: " << stack2.IsEmpty() << endl;

	return 0;
}
