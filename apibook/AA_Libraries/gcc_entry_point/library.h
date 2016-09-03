/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   library.h
/// \author Martin Reddy
/// \brief  An example of a shared library with entry points
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>

extern "C" {

void DoSomething(const std::string &name);

}

#endif
