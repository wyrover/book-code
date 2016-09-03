/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   stack.cpp
/// \author Martin Reddy
/// \brief  An example of explicit template instantiation.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "stack.h"

namespace apibook {

template <typename T>
void Stack<T>::Push(T val)
{
	mStack.push_back(val);
}

template <typename T>
T Stack<T>::Pop()
{
	if (IsEmpty())
		return T();
	T val = mStack.back();
	mStack.pop_back();
	return val;
}

template <typename T>
bool Stack<T>::IsEmpty() const
{
	return mStack.empty();
}

// explicit template instantiation
template class Stack<int>;

}
