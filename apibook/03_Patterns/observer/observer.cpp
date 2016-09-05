/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   observer.cpp
/// \author Martin Reddy
/// \brief  Abstract base classes to implement Observer.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "observer.h"

ISubject::ISubject() {}
ISubject::~ISubject() {}

void ISubject::Subscribe(int message, IObserver *observer)
{
	if (observer)
	{
		ObserverMap::iterator it = mObservers.find(message);
		if (it == mObservers.end())
		{
			mObservers[message] = ObserverList();
		}
		mObservers[message].push_back(observer);
	}
}

void ISubject::Unsubscribe(int message, IObserver *observer)
{
	ObserverMap::iterator it = mObservers.find(message);
	if (it != mObservers.end())
	{
		ObserverList &list = mObservers[message];
		ObserverList::iterator li;
		for (li = list.begin(); li != list.end();)
		{
			if ((*li) == observer)
			{
				list.erase(li);
			}
			else
			{
				++li;
			}
		}
	}
}

void ISubject::Notify(int message)
{
	ObserverMap::iterator it = mObservers.find(message);
	if (it != mObservers.end())
	{
		ObserverList &list = mObservers[message];
		ObserverList::iterator li;
		for (li = list.begin(); li != list.end(); ++li)
		{
			(*li)->Update(message);
		}
	}
}


