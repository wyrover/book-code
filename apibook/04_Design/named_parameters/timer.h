/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   timer.h
/// \author Martin Reddy
/// \brief  Example of the Named Parameter Idiom (NPI)
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef TIMER
#define TIMER

namespace apibook {

///
/// A simple class that demonstrates the Named Parameter Idiom.
///
class Timer
{
public:
	Timer();
	~Timer();

	/// Allow this class to be copyable
	Timer(const Timer &);
	const Timer &operator =(const Timer &);

	/// An NPI-style setter for the timer delay
	Timer &SetDelay(float t);
	/// An NPI-style setter for whether the timer repeats
	Timer &SetSingleShot(bool ss);

	/// Start the timer with the current settings
	Timer &Start();

private:
	class Impl;
	Impl *mImpl;
};

}

#endif
