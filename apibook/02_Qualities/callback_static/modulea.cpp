/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   modulea.cpp
/// \author Martin Reddy
/// \brief  A module that declares a static callback method.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "modulea.h"
#include <iostream>

using std::cout;
using std::endl;

namespace apibook {

void ModuleA::StaticMethod(const std::string &name, void *data)
{
	int closure = *static_cast<int *>(data);
	cout << "In static callback: '" << name << "', closure=" << closure << endl;
}

}
