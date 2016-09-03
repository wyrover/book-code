/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  An example of the Observer design pattern.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "subject.h"
#include <iostream>
#include <string>

using std::cout;
using std::endl;

/// An observer class that wishes to receive a notification
class MyObserver : public IObserver
{
public:
	MyObserver(const std::string &str) :
		mName(str)
	{}

	void Update(int message)
	{
		cout << mName << " Received message " << message << endl;
	}

private:
	std::string mName;
};

int main(int, char **)
{
	// create three observer classes
	MyObserver observer1("observer1");
	MyObserver observer2("observer2");
	MyObserver observer3("observer3");
	// create a single subject class
	MySubject subject;

	// each observer registers interest for specific
	// messages that the subject can issue
	subject.Subscribe(MySubject::ADD, &observer1);
	subject.Subscribe(MySubject::ADD, &observer2);
	subject.Subscribe(MySubject::REMOVE, &observer2);
	subject.Subscribe(MySubject::UPDATE, &observer1);
	subject.Subscribe(MySubject::UPDATE, &observer3);

	// make the subject issue ADD and UPDATE messages
	cout << "Notify ADD:" << endl;
	subject.Notify(MySubject::ADD);
	cout << "Notify UPDATE:" << endl;
	subject.Notify(MySubject::UPDATE);

	return 0;
}
