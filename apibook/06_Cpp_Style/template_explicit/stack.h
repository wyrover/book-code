/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   stack.h
/// \author Martin Reddy
/// \brief  An example of explicit template instantiation.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef STACK_H
#define STACK_H

#include <vector>

namespace apibook {

///
/// A class template to create a stack of objects
///
template <typename T>
class Stack
{
public:
	/// Push a new value to the stack
	void Push(T val);
	/// Pop the last value off the stack
	T Pop();
	/// Return true if the stack contains no values
	bool IsEmpty() const;

private:
	std::vector<T> mStack;
};

}

#endif
