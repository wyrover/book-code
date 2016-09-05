/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   proxy.h
/// \author Martin Reddy
/// \brief  A simple example of a Proxy design pattern.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef PROXY_H
#define PROXY_H

class Original;

///
/// A Proxy class that wraps access to an Original class.
///
class Proxy
{
public:
	Proxy();
	~Proxy();

	/// Call through to the Original::DoSomething() function
	bool DoSomething(int value);

private:
	// prevent copying of this class because we had a pointer data member
	Proxy(const Proxy &);
	const Proxy &operator =(const Proxy &);

	Original *mOrig;
};

#endif
