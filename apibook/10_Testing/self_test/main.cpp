/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  Show a self test function to test private funcs.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "bbox.h"

int main(int, char **)
{
	BBox bbox;

	// call public SelfTest() method to test internal details
	bbox.SelfTest();

	return 0;
}
