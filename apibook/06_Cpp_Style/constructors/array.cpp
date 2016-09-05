/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   array.cpp
/// \author Martin Reddy
/// \brief  An example of using constructors and assignment.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "array.h"
#include <iostream>
#include <algorithm>

namespace apibook {

// default constructor
Array::Array() :
	mSize(0),
	mArray(NULL)
{
	std::cout << "default constructor" << std::endl;
}

// non-default constructor
Array::Array(int size) :
	mSize(size),
	mArray(new std::string[size])
{
	std::cout << "non-default constructor" << std::endl;
}

// destructor
Array::~Array()
{
	std::cout << "destructor" << std::endl;
	delete [] mArray;
}

// copy constructor
Array::Array(const Array &in_array) :
	mSize(in_array.mSize),
	mArray(new std::string[in_array.mSize])
{
	std::cout << "copy constructor" << std::endl;
	std::copy(in_array.mArray, in_array.mArray + mSize, mArray);
}

// assignment operator
Array &Array::operator = (const Array &in_array)
{
	std::cout << "assignment operator" << std::endl;
	if (this != &in_array)
	{
		delete [] mArray; // delete current array first

		mSize = in_array.mSize;
		mArray = new std::string[in_array.mSize];
		std::copy(in_array.mArray, in_array.mArray + mSize, mArray);
	}

	return *this;
}

std::string Array::Get(int index) const
{
	if (index < 0 || index >= mSize) 
	{
		return std::string("");
	}

	return mArray[index];
}

bool Array::Set(int index, const std::string &str)
{
	if (index < 0 || index >= mSize) 
	{
		return false;
	}

	mArray[index] = str;
	return true;
}

int Array::Size() const
{
	return mSize;
}

void Array::Print() const
{
	std::cout << "Size = " << mSize << std::endl;
	for (int i = 0; i < mSize; ++i)
	{
		std::cout << i << ": " << mArray[i] << std::endl;
	}
}

}

