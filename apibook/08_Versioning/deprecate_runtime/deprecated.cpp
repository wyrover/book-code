/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   deprecate.cpp
/// \author Martin Reddy
/// \brief  Function to deprecate functionality.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "deprecated.h"
#include <iostream>
#include <set>

using std::cout;
using std::endl;

void Deprecated(const std::string oldfunc, const std::string newfunc)
{
	static std::set<std::string> sAlreadySeen;
	if (sAlreadySeen.find(oldfunc) != sAlreadySeen.end())
	{
		return;
	}

	cout << "WARNING: " << oldfunc << " is deprecated.";
	if (! newfunc.empty())
	{
		cout << " Use " << newfunc << " instead.";
	}
	cout << endl;

	sAlreadySeen.insert(oldfunc);
}
