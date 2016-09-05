/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   curious_priv.h
/// \author Martin Reddy
/// \brief  The curiously recurring template pattern (CRTP)
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef CURIOUS_PRIV_H
#define CURIOUS_PRIV_H

namespace apibook {

template <typename TrackedType> 
size_t MemoryTracker<TrackedType>::bytesUsed() { 
	return MemoryTracker<TrackedType>::bytes; 
} 

template <typename TrackedType> 
MemoryTracker<TrackedType>::MemoryTracker() { 
	MemoryTracker<TrackedType>::bytes += objsize();
} 

template <typename TrackedType> 
MemoryTracker<TrackedType>::MemoryTracker (MemoryTracker<TrackedType> const&) { 
	MemoryTracker<TrackedType>::bytes += objsize();
} 

template <typename TrackedType> 
MemoryTracker<TrackedType>::~MemoryTracker() { 
	MemoryTracker<TrackedType>::bytes -= objsize();
} 

template <typename TrackedType> 
inline size_t MemoryTracker<TrackedType>::objsize() {
	return sizeof(*static_cast<TrackedType *>(this));
}

 
// initialize counter with zero 
template <typename TrackedType> 
size_t MemoryTracker<TrackedType>::bytes = 0; 

}

#endif
