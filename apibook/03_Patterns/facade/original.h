/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   original.h
/// \author Martin Reddy
/// \brief  An original class to be wrapped by a Facade.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef ORIGINAL_H
#define ORIGINAL_H

//
// One class in the sub-system wrapped by Facade.
//
class Original1
{
public:
	int DoOperation1();
};

//
// Another class in the sub-system wrapped by Facade.
//
class Original2
{
public:
	bool DoOperation2(int value);
};

#endif
