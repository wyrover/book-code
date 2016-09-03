/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   myobject.h
/// \author Martin Reddy
/// \brief  A simple class to demonstrate shared pointers.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <boost/shared_ptr.hpp>

namespace apibook {

/// A reference-counted pointer to MyObject
typedef boost::shared_ptr<class MyObject> MyObjectPtr;

///
/// A class that lets you create reference-counted
/// pointers to itself via a factory method. The
/// constructor is declared to be private, so you
/// have to use CreateInstance() instead of new to
/// create instances of MyObject.
///
class MyObject
{
public:
	/// Create a new (reference-counted) instance of MyObject
	static MyObjectPtr CreateInstance(int id);
	~MyObject();

private:
	MyObject(int id);
	int mID;
};

}

#endif
