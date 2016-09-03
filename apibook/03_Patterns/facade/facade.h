/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   facade.h
/// \author Martin Reddy
/// \brief  An example of the Facade design pattern.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef FACADE_H
#define FACADE_H

///
/// An facade that wraps access to a sub-system.
///
class Facade
{
public:
	Facade();
	~Facade();

	/// Call various functions in the sub-system to aggregate its functionality
	bool DoSomething();

private:
	Facade(const Facade &);
	const Facade &operator =(const Facade &);

	class Impl;
	Impl *mImpl;
};

#endif
