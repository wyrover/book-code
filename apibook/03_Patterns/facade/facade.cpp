/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   facade.cpp
/// \author Martin Reddy
/// \brief  An example of the Facade design pattern.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "facade.h"
#include "original.h"
#include <iostream>

using std::cout;
using std::endl;

class Facade::Impl
{
public:
	Impl() :
		mOriginal1(NULL),
		mOriginal2(NULL)
	{
	}

	~Impl()
	{
		delete mOriginal1;
		delete mOriginal2;
		cout << "Destroyed Original objects inside Facade" << endl;
	}

	Original1 *GetOriginal1()
	{
		if (! mOriginal1)
		{
			cout << "Lazily allocating Original1" << endl;
			mOriginal1 = new Original1;
		}
		return mOriginal1;
	}

	Original2 *GetOriginal2()
	{
		if (! mOriginal2)
		{
			cout << "Lazily allocating Original2" << endl;
			mOriginal2 = new Original2;
		}
		return mOriginal2;
	}

private:
	Original1 *mOriginal1;
	Original2 *mOriginal2;
};

Facade::Facade()
	: mImpl(new Facade::Impl)
{
}

Facade::~Facade()
{
	delete mImpl;
}

bool Facade::DoSomething()
{
	cout << "About to call routines in Original1 and Original2" << endl;
	int result = mImpl->GetOriginal1()->DoOperation1();
	if (result < 100)
		return mImpl->GetOriginal2()->DoOperation2(result);
	return false;
}

