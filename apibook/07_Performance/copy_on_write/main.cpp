/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  A class template with copy-on-write behavior. 
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "cowptr.h"
#include <string>
#include <iostream>

using namespace apibook;
using std::cout;
using std::endl;

class MyObject
{
public:
	MyObject() : mData(0) {}

	std::string GetValue() const
	{
		return (mData) ? *mData : "";
	}
	void SetValue(const std::string &value)
	{
		mData = new std::string(value);
	}
	void Print(const std::string &title) const
	{
		const std::string *ptr = mData.data();
		cout << title << "(" << ptr << "): ";
		cout << ((ptr) ? *ptr : "(null)") << endl;
	}

private:
	CowPtr<std::string> mData;
};

int main(int, char **)
{
	CowPtr<std::string> string1(new std::string("Share Me"));
	char &char_ref = string1->operator[](1);
	CowPtr<std::string> string2(string1);
	char_ref = 'p'; // affects both string1 and string2
	cout << "Tried to fool CowPtr - but strings should be using different pointers" << endl;
	cout << "String1 = " << string1 << endl;
	cout << "String2 = " << string2 << endl;

	MyObject obj1;
	obj1.SetValue("Hello");

	MyObject obj2 = obj1;
	std::string val = obj2.GetValue();

	MyObject obj3 = obj1;
	obj3.SetValue("There");

	cout << "obj1 and obj2 should share the same pointer" << endl;
	obj1.Print("obj1");
	obj2.Print("obj2");
	cout << "obj3 is modified so should be a different pointer" << endl;
	obj3.Print("obj3");

	return 0;
}
