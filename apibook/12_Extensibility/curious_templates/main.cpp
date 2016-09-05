/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  Demonstrate the use of the CRTP.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "curious.h"
#include <iostream>

using namespace apibook;

// A class template that uses the MemoryTracker CRTP
template <typename T> 
class MyClass1 : public MemoryTracker<MyClass1<T> > { 
public:
	int mValue;
}; 

// A plain class that uses the MemoryTracker CRTP
class MyClass2 : public MemoryTracker<MyClass2> { 
public:
	int mValue;
}; 

int main(int, char **)
{
	// create several instances of each class
	MyClass1<char> s1, s2; 
	MyClass1<wchar_t> ws; 
	MyClass2 foo, bar, baz;

	// report how much memory is used by each type
	std::cout << "number of MyClass1<char>: " 
			  << MyClass1<char>::bytesUsed() << std::endl; 
	std::cout << "number of MyClass1<wchar_t>: " 
			  << ws.bytesUsed() << std::endl; 
	std::cout << "number of MyClass2: " 
			  << foo.bytesUsed() << std::endl; 

	return 0;
}
