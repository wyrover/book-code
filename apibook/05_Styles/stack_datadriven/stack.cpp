/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   stack.cpp
/// \author Martin Reddy
/// \brief  An example of a data-driven style API. 
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "stack.h"
#include <iostream>

using std::cout;
using std::endl;

namespace apibook {

Arg Stack::Command(const std::string &cmd, const ArgList &args)
{
	if (cmd == "Push")
	{
		mStack.push_back(args.Get("value").ToInt());
		return Arg(true);
	}

	if (cmd == "Pop")
	{
		if (mStack.empty())
			return Arg(0);
		int val = mStack.back();
		mStack.pop_back();
		return Arg(val);
	}

	if (cmd == "IsEmpty")
	{
		return Arg(mStack.empty());
	}

	cout << "ERROR: unknown command for Stack" << endl;
	return Arg();
}


}

