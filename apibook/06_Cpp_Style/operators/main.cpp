/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  Demonstrating good C++ operator style.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "currency.h"
#include <iostream>

int main(int, char **)
{
	// use one of the operator functions of Currency
	apibook::Currency c1(100);
	apibook::Currency c2(150);

	c1 += c2;

	std::cout << "Currency: " << c1 << std::endl;

	return 0;
}
