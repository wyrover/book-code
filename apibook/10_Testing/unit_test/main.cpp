/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  An example of a unit test.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "stringutil.h"
#include <iostream>

using std::cout;
using std::endl;

// define some simple assertion routines
void Assert(bool result, const std::string &desc)
{
	if (result)
		return;
	cout << "FAIL: " << desc << endl;
}

void AssertFalse(bool result, const std::string &desc)
{
	Assert(! result, desc);
}

void AssertEqual(double val1, double val2, const std::string &desc)
{
	if (val1 == val2)
		return;
	cout << "FAIL: " << desc << endl;
	cout << val1 << " != " << val2 << endl;
}

// the actual unit test
void Test_StringToDouble()
{
	double value;

	Assert(StringToDouble("1", value), "+ve test");
	AssertEqual(value, 1.0, "'1' == 1.0");

	Assert(StringToDouble("-1", value), "-ve test");
	AssertEqual(value, -1.0, "'-1' == -1.0");

	Assert(StringToDouble("0.0", value), "zero");
	AssertEqual(value, 0.0, "'0.0' == 0.0");

	Assert(StringToDouble("-0", value), "minus zero");
	AssertEqual(value, -0.0, "'-0' == -0.0");
	AssertEqual(value, 0.0, "'-0' == 0.0");

	Assert(StringToDouble("3.14159265", value), "pi");
	AssertEqual(value, 3.14159265, "pi value");

	Assert(StringToDouble("2e10", value), "+ve scientific");
	AssertEqual(value, 2e10, "");

	Assert(StringToDouble("+4.3e-10", value), "-ve scientific");
	AssertEqual(value, 4.3e-10, "");

	AssertFalse(StringToDouble("", value), "empty");
	AssertFalse(StringToDouble("   ", value), "whitespace");
	AssertFalse(StringToDouble("+-1", value), "+-");
	AssertFalse(StringToDouble("1.1.0", value), "multiple points");
	AssertFalse(StringToDouble("text", value), "not a number");

	cout << "SUCCESS! Unit test passed." << endl;
}

int main(int, char **)
{
	Test_StringToDouble();
	return 0;
}
