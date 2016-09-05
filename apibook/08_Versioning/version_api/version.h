/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   version.h
/// \author Martin Reddy
/// \brief  Describe an API's version information.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef VERSION_H
#define VERSION_H

#include <string>

#define API_MAJOR 1
#define API_MINOR 2
#define API_PATCH 0

///
/// Version information for an API
///
class Version
{
public:
	/// Return the major version number, e.g., 1 for "1.2.3"
	static int GetMajor();
	/// Return the minor version number, e.g., 2 for "1.2.3"
	static int GetMinor();
	/// Return the patch version number, e.g., 3 for "1.2.3"
	static int GetPatch();

	/// Return the full version number as a string, e.g., "1.2.3"
	static std::string GetVersion();

	/// Return true if the current version >= (major, minor, patch)
	static bool IsAtLeast(int major, int minor, int patch);

	/// Return true if the named feature is available in this version
	static bool HasFeature(const std::string &name);
};

#endif
