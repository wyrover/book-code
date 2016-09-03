/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   stack.cpp
/// \author Martin Reddy
/// \brief  An example of a pre-processor based API.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "stack.h"

#define DEFINE_STACK(Prefix, T) \
void Prefix##Stack::Push(T val) \
{ \
	mStack.push_back(val); \
} \
T Prefix##Stack::Pop() \
{ \
	if (IsEmpty()) \
		return T(); \
	T val = mStack.back(); \
	mStack.pop_back(); \
	return val; \
} \
bool Prefix##Stack::IsEmpty() const \
{ \
	return mStack.empty(); \
}

DEFINE_STACK(Int, int);
DEFINE_STACK(String, std::string);
DEFINE_STACK(Double, double);
