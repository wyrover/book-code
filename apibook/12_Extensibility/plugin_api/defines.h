/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   defines.h
/// \author Martin Reddy
/// \brief  Win32 decorator macros for the Core and Plugin APIs
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef DEFINES_H
#define DEFINES_H

#ifdef _WIN32
#ifdef BUILDING_CORE
#define CORE_API   __declspec(dllexport)
#define PLUGIN_API __declspec(dllimport)
#else
#define CORE_API   __declspec(dllimport)
#define PLUGIN_API __declspec(dllexport)
#endif
#else
#define CORE_API
#define PLUGIN_API
#endif

#ifdef _WIN32
#pragma warning ( disable : 4251 )
#endif

#endif
