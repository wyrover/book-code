/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file    version.h
///
/// \brief   Access the API's version information.
///
/// \author  Martin Reddy
/// \date    2010-07-07
/// \since   1.0
/// \ingroup group_Versioning
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
///

#ifndef VERSION_H
#define VERSION_H

#include <string>

///
/// \class Version version.h API/version.h
///
/// \brief Access the version information for the API
///
/// For example, you can get the current version number as
/// a string using \c GetVersion, or you can get the separate
/// major, minor, and patch integer values by calling
/// \c GetMajor, \c GetMinor, or \c GetPatch, respectively.
///
/// This class also provides some basic version comparison
/// functionality and lets you determine if certain named
/// features are present in your current build.
///
/// \author Martin Reddy
/// \date   2010-07-07
/// \since 1.0
///
class Version
{
public:
	/// \name Version Numbers
	//@{
	///
	/// \brief Return the API major version number.
	/// \return The major version number as an integer.
	/// \since 1.0
	///
	static int GetMajor();

	///
	/// \brief Return the API minor version number.
	/// \return The minor version number as an integer.
	/// \since 1.0
	///
	static int GetMinor();

	///
	/// \brief Return the API patch version number.
	/// \return The patch version number as an integer.
	/// \since 1.0
	///
	static int GetPatch();

	///
	/// \brief Return the API full version number.
	/// \return The version string, e.g., "1.0.1".
	/// \since 1.0
	///
	static std::string GetVersion();
	//@}

	/// \name Version Number Math
	//@{
	///
	/// \brief Compare the current version number against a specific
	///        version.
	///
	/// This method let's you check to see if the current version
	/// is greater than or equal to the specified version. This may
	/// be useful to perform operations that require a minimum
	/// version number.
	///
	/// \param[in] major The major version number to compare against
	/// \param[in] minor The minor version number to compare against
	/// \param[in] patch The patch version number to compare against
	/// \return Returns true if specified version >= current version
	/// \since 1.0
	///
	static bool IsAtLeast(int major, int minor, int patch);
	//@}

	/// \name Feature Tags
	//@{
	///
	/// \brief Test whether a feature is implemented by this API.
	///
	/// New features that change the implementation of API methods
	/// are specified as a "feature tag". This method lets you 
	/// query the API to find out if a given feature is available.
	///
	/// \param[in] name The feature tag name, e.g., "LOCKING"
	/// \return Returns true if the named feature is available.
	/// \since 1.0
	///
	static bool HasFeature(const std::string &name);
	//@}
};

#endif
