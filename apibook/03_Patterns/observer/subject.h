/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   subject.h
/// \author Martin Reddy
/// \brief  The subject of an observer relationship.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef SUBJECT_H
#define SUBJECT_H

#include "observer.h"

///
/// A concrete subject class, based on the ISubject interface.
/// 
class MySubject : public ISubject
{
public:
	enum Messages { ADD, REMOVE, UPDATE };
};

#endif
