/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  Demo a class to manage the version of an API.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "version.h"
#include <iostream>

using std::cout;
using std::endl;

int main(int, char **)
{
	cout << "Version: " << Version::GetVersion() << endl;

	cout << "AtLeast 1.2.0: " << Version::IsAtLeast(1,2,0) << endl;
	cout << "AtLeast 2.1.0: " << Version::IsAtLeast(2,1,0) << endl;
	cout << "AtLeast 1.1.9: " << Version::IsAtLeast(1,1,9) << endl;

	cout << "Feature FOOBAR: " << Version::HasFeature("FOOBAR") << endl;
	cout << "Feature FASTAPI: " << Version::HasFeature("FASTAPI") << endl;

	return 0;
}
