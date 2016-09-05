/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  Demonstrate run-time deprecation.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "myclass.h"

int main(int, char **)
{
	MyClass cls;
	
	// can still call deprecated method, but will get a run-time warning
	cls.GetName();

	return 0;
}
