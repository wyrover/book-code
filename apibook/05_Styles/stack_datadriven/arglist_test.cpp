/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   arglist_test.cpp
/// \author Martin Reddy
/// \brief  An example program using the ArgList API.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "arglist.h"
#include <iostream>

using std::cout;
using std::endl;
using namespace apibook;

int main(int, char **)
{
	cout << "Arg(10):" << endl;
	Arg arg(10);
	cout << "Arg is bool: " << arg.ContainsBool() << endl;
	cout << "Arg is int: " << arg.ContainsInt() << endl;
	cout << "Arg as string: " << arg.ToString() << endl;

	cout << "Arg(true):" << endl;
	arg.Set(true);
	cout << "Arg is bool: " << arg.ContainsBool() << endl;
	cout << "Arg is int: " << arg.ContainsInt() << endl;
	cout << "Arg as string: " << arg.ToString() << endl;

	cout << "ArgList():" << endl;
	ArgList args = ArgList().Add("int", 7).Add("bool",true).Add("string", std::string("Hello"));

	cout << "Arg 'int' = " << args.Get("int").ToString() << endl;
	cout << "Arg 'bool' = " << args.Get("bool").ToString() << endl;
	cout << "Arg 'string' = " << args.Get("string").ToString() << endl;
	
	return 0;
}
