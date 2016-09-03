/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  A poor API that can be improved by Pimpl.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "autotimer.h"
#include <iostream>

using std::cout;
using std::endl;

int main(int, char **)
{
	apibook::AutoTimer timer("MyTimer");

	for (int i = 0; i < 1000; ++i)
	{
		cout << ".";
	}
	cout << endl;

	return 0;
}
