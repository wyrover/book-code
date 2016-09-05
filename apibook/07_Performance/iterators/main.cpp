/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  Demonstrate the overhead of using iterators.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "value.h"
#include <vector>
#include <iostream>
#include <numeric>

using std::cout;
using std::endl;

int main(int, char **)
{
	// create a vector with three Value objects
	std::vector<Value> values;
	values.push_back(Value(10));
	values.push_back(Value(-2));
	values.push_back(Value(8));

	// use an STL iterator to iterate through the vector
	cout << "START ITERATION" << endl;

	int sum = 0;
	std::vector<Value>::const_iterator it;
	for (it = values.begin(); it != values.end(); ++it)
	{
		const Value &value = *it;
		sum += (*it).Get();
		(void) value; // avoid compiler warning
	}

	cout << "END ITERATION: " << sum << endl;

	return 0;
}
