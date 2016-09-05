/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   version.cpp
/// \author Martin Reddy
/// \brief  Describe an API's version information.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "version.h"
#include <sstream>
#include <string>
#include <set>

int Version::GetMajor()
{
	return int(API_MAJOR);
}

int Version::GetMinor()
{
	return int(API_MINOR);
}

int Version::GetPatch()
{
	return int(API_PATCH);
}

std::string Version::GetVersion()
{
	static std::string version("");

	if (version.empty())
	{
		// cache the version string
		std::ostringstream stream;
		stream << API_MAJOR << "."
			   << API_MINOR << "."
			   << API_PATCH;
		version = stream.str();
	}

	return version;
}

bool Version::IsAtLeast(int major, int minor, int patch)
{
	if (API_MAJOR < major) return false;
	if (API_MAJOR > major) return true;
	if (API_MINOR < minor) return false;
	if (API_MINOR > minor) return true;
	if (API_PATCH < patch) return false;
	return true;
}

bool Version::HasFeature(const std::string &name)
{
	static std::set<std::string> features;

	if (features.empty())
	{
		// cache the feature list
		features.insert("FASTAPI");
		features.insert("THREADSAFE");
	}

	return features.find(name) != features.end();
}

