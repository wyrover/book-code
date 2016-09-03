/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  An example of explicit template instantiation.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "stack.h"
#include <iostream>

int main(int, char **)
{
	// create a specialization of Stack for ints
	apibook::Stack<int> stack;

	// push and pop a value to the stack
	std::cout << "Empty: " << stack.IsEmpty() << std::endl;
	stack.Push(10);
	std::cout << "Empty: " << stack.IsEmpty() << std::endl;
	int val = stack.Pop();
	std::cout << "Popped off: " << val << std::endl;
	std::cout << "Empty: " << stack.IsEmpty() << std::endl;

	return 0;
}
