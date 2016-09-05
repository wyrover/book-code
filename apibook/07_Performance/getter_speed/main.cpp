/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  Time the speed of various data member accesses.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "vector.h"
#include "autotimer.h"
#include <stdio.h>
#include <string>

static const std::string filename = "getter_speed.tmp";

static void useVector(double x, double y, double z)
{
	// use the input parameters in such as way that the compiler can't
	// optimize the inner computation loops in main() away
	FILE *fp = NULL;
#ifdef _WIN32
	fopen_s(&fp, filename.c_str(), "w");
#else
	fp = fopen(filename.c_str(), "w");
#endif
	if (fp) {
		fprintf(fp, "%f %f %f\n", x, y, z);
		fclose(fp);
	}
}

static void cleanup()
{
#ifdef _WIN32
	_unlink(filename.c_str());
#else
	unlink(filename.c_str());
#endif
}

int main(int, char **)
{
	size_t iterations = 0;

	// work out roughly how many iterations we can do in 1 sec
	{
		AutoTimer timer("Calibration");
		ExposedVector vec(1.0, 2.0, 3.0);
		while (true) {
			vec.mX = vec.mX * vec.mY + vec.mZ;
			vec.mZ = vec.mZ / vec.mY + vec.mX;
			if (iterations++ % 1000 == 0 && timer.GetElapsed() >= 1.0) {
				break;
			}
		}
	}

	// time computations on a class with exposed member variables
	{
		AutoTimer timer("ExposedVector");
		ExposedVector vec(1.0, 2.0, 3.0);
		for (size_t i = 0; i < iterations; i++) {
			vec.mX = vec.mX * vec.mY + vec.mZ;
			vec.mZ = vec.mZ / vec.mY + vec.mX;
		}
		useVector(vec.mX, vec.mY, vec.mZ);
	}

	// time computations on a class using gettings/setters
	{
		AutoTimer timer("HiddenVector");
		HiddenVector vec(1.0, 2.0, 3.0);
		for (size_t i = 0; i < iterations; i++) {
			vec.setX(vec.getX() * vec.getY() + vec.getZ());
			vec.setZ(vec.getZ() / vec.getY() + vec.getX());
		}
		useVector(vec.getX(), vec.getY(), vec.getZ());
	}

	// time computations on a class using inlined gettings/setters
	{
		AutoTimer timer("InlinedVector");
		InlinedVector vec(1.0, 2.0, 3.0);
		for (size_t i = 0; i < iterations; i++) {
			vec.setX(vec.getX() * vec.getY() + vec.getZ());
			vec.setZ(vec.getZ() / vec.getY() + vec.getX());
		}
		useVector(vec.getX(), vec.getY(), vec.getZ());
	}

	cleanup();

	return 0;
}
