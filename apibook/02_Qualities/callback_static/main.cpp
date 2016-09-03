/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  ModuleB calls a static method on ModuleA.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "modulea.h"
#include "moduleb.h"

int main(int, char **)
{
	apibook::ModuleB b;
	int              closure = 42;

	// add a callback for ModuleA to ModuleB
	b.SetCallback(apibook::ModuleA::StaticMethod, &closure);
	// tell ModuleB to invoke all its callbacks
	b.DoAction();

	return 0;
}
