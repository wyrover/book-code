/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   curious.h
/// \author Martin Reddy
/// \brief  The curiously recurring template pattern (CRTP)
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef CURIOUS_H
#define CURIOUS_H

#include <stddef.h> 
#include <stdio.h>

namespace apibook {

///
/// A class template that uses the CRTP to track the 
/// memory usage of all specializations.
///
template <typename TrackedType> 
class MemoryTracker { 
public:
	/// return memory used by existing objects
	static size_t bytesUsed();

protected:
	/// default constructor 
	MemoryTracker();
	
	/// copy constructor 
	MemoryTracker (MemoryTracker<TrackedType> const&);
 
	/// destructor 
	virtual ~MemoryTracker();

private: 
	size_t objsize();
	static size_t bytes;  // number of existing objects  
}; 

}

#include "curious_priv.h"

#endif
