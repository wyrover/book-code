/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   library.cpp
/// \author Martin Reddy
/// \brief  An example of a shared library with entry points
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "library.h"

#include <iostream>

using std::cout;
using std::endl;

///
/// A class that holds the library init/cleanup functions
///
class APIInitMgr
{
public:
	APIInitMgr()
	{
		cout << "APIInitMgr Initialized." << endl;
	}

	~APIInitMgr()
	{
		cout << "APIInitMgr Destroyed." << endl;
	}
};

static APIInitMgr foo = APIInitMgr();

static void __attribute__ ((constructor)) APIInitialize()
{
	cout << "API Initialized." << endl;
}

static void __attribute__ ((destructor)) APICleanup()
{
	cout << "API Cleaned Up." << endl;
}

void DoSomething(const std::string &name)
{
	cout << "Plugin says: " << name << endl;
}

