/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   modulea.h
/// \author Martin Reddy
/// \brief  A module that uses one of its methods as a callback.
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
/// module. To simplify calling an member function for a
/// specific instance of this class, a static wrapper function
/// can be used for the callback. This essentially accepts the
/// object instance as an additional parameter.
///
class ModuleA
{
public:
    /// A static function that lets you call a member function
	static void StaticWrapper(void *obj, const std::string &name, void *data);

    /// The member function callback
	void InstanceMethod(const std::string &name, void *data);
};

}

#endif
