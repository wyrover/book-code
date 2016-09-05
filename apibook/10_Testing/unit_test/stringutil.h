/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   stringutil.h
/// \author Martin Reddy
/// \brief  A string conversion utility to be unit tested.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>

/// Convert a string to a double and return true if successful
bool StringToDouble(const std::string &str, double &result); 

#endif
