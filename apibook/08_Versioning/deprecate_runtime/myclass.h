/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   myclass.h
/// \author Martin Reddy
/// \brief  Example class to demonstrate deprecation.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef MYCLASS_H
#define MYCLASS_H

#include <string>

class MyClass
{
public:
	std::string GetFullName();

	/// Deprecated: emits a run-time (one-time) warning
	std::string GetName();
};

#endif
