/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   original.h
/// \author Martin Reddy
/// \brief  An original class to be wrapped by a Proxy.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef ORIGINAL_H
#define ORIGINAL_H

///
/// An Original class wrapped by Proxy.
///
class Original
{
public:
	bool DoSomething(int value);
};

#endif
