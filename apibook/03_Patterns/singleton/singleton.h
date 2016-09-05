/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   singleton.h
/// \author Martin Reddy
/// \brief  An example of the Singleton design pattern.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef SINGLETON_H
#define SINGLETON_H

#include <string>

///
/// A class that enforces the policy that only one
/// instance is ever created.
///
class Singleton
{
public:
	/// Return the single instance of this class
	static Singleton &GetInstance();

	/// Return some class-specific single-instance state
	int GetState();

private:
	// Prevent construction and copying of this class
	Singleton();
	~Singleton();
	Singleton(const Singleton &);
	const Singleton &operator =(const Singleton &);
};

#endif
