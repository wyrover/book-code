/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   moduleb.h
/// \author Martin Reddy
/// \brief  A module that invokes a callback on an object.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "moduleb.h"

namespace apibook {

ModuleB::ModuleB() : mObject(NULL), mCallback(NULL), mClosure(NULL)
{
}

void ModuleB::SetCallback(void *obj, CallbackType cb, void *data)
{
	mObject = obj;
	mCallback = cb;
	mClosure = data;
}

void ModuleB::DoAction()
{
	if (mCallback)
	{
		(*mCallback)(mObject, "Hello World", mClosure);
	}
}

}
