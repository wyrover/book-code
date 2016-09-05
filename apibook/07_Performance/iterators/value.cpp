/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   value.cpp
/// \author Martin Reddy
/// \brief  A simple copyable class with getter/setter.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "value.h"
#include <iostream>
#include <algorithm>

// default constructor
Value::Value() :
	mValue(NULL)
{
	std::cout << "default constructor" << std::endl;
}

// non-default constructor
Value::Value(int value) :
	mValue(value)
{
	std::cout << "non-default constructor" << std::endl;
}

// destructor
Value::~Value()
{
	std::cout << "destructor" << std::endl;
}

// copy constructor
Value::Value(const Value &in_array) :
	mValue(in_array.mValue)
{
	std::cout << "copy constructor" << std::endl;
}

// assignment operator
Value &Value::operator = (const Value &in_array)
{
	std::cout << "assignment operator" << std::endl;
	if (this != &in_array)
	{
		mValue = in_array.mValue;
	}

	return *this;
}

int Value::Get() const
{
	return mValue;
}

void Value::Set(int value)
{
	mValue = value;
}

