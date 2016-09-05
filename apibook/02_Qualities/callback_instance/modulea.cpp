/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   modulea.cpp
/// \author Martin Reddy
/// \brief  A module that uses one of its methods as a callback.
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

void ModuleA::StaticWrapper(void *obj, const std::string &name, void *data)
{
	ModuleA *self = static_cast<ModuleA *>(obj);
	self->InstanceMethod(name, data);
}

void ModuleA::InstanceMethod(const std::string &name, void *data)
{
	int closure = *static_cast<int *>(data);
	cout << "In instance callback: '" << name << "' (closure=" << closure << ")" << endl;
}

}
