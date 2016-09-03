/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   moduleb.cpp
/// \author Martin Reddy
/// \brief  A module that invokes a static callback method.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "moduleb.h"

namespace apibook {

ModuleB::ModuleB() : mCallback(NULL), mClosure(NULL)
{
}

void ModuleB::SetCallback(CallbackType cb, void *data)
{
	mCallback = cb;
	mClosure = data;
}

void ModuleB::DoAction()
{
	if (mCallback)
	{
		(*mCallback)("Hello World", mClosure);
	}
}

}
