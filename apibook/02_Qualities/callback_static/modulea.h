/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   modulea.h
/// \author Martin Reddy
/// \brief  A module that declares a static callback method.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef MODULEA_H
#define MODULEA_H

#include <string>

namespace apibook {

///
/// An object that wishes to receive a callback from another
/// module. A static function is used to avoid the complication
/// of calling a function on a specific instance of the object.
///
class ModuleA
{
public:
    /// The static function callback
	static void StaticMethod(const std::string &name, void *data);
};

}

#endif
