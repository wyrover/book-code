/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   moduleb.h
/// \author Martin Reddy
/// \brief  A module that invokes a static callback method.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef MODULEB_H
#define MODULEB_H

#include <string>
#include <stdlib.h>

namespace apibook {

///
/// An object that can register a callback function and then
/// invoke that callback when desired.
///
class ModuleB
{
public:
	/// A type for the function signature that is accepted as a callback
	typedef void (*CallbackType)(const std::string &name, void *data);

	ModuleB();

	/// Set the callback function. Only one callback can be registered.
	void SetCallback(CallbackType cb, void *data);
	/// Call the callback function.
	void DoAction();

private:
	CallbackType mCallback;
	void         *mClosure;
};

}

#endif
