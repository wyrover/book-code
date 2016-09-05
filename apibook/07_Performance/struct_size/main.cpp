/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  Show memory size for versions of the same class.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include <iostream>
#include <string.h>

using std::cout;
using std::endl;

#define INITIALIZE_MEMBERS(VarianceType, OriginType) \
		memset(this, 0, sizeof(*this)); \
		mIsActive = 0x01; \
		mVaryColor = 0x01; \
		mRepeatCycle = 0x01f; \
		mFadeParticles = 0x01f; \
		mRed = 0x02; \
		mGreen = 0x02; \
		mBlue = 0x02; \
		mRedVariance = (VarianceType)0x03030303;	\
		mGreenVariance = (VarianceType)0x03030303; \
		mBlueVariance = (VarianceType)0x03030303; \
		mTotalParticles = 0x03030303; \
		mOriginX = (OriginType)0x03030303; \
		mOriginY = (OriginType)0x03030303


class Fireworks_A
{
public:
	Fireworks_A()
	{
		INITIALIZE_MEMBERS(int, int);
	}
	bool mIsActive;
	int mOriginX;
	int mOriginY;
	bool mVaryColor;
	char mRed;
	int mRedVariance;
	char mGreen;
	int mGreenVariance;
	char mBlue;
	int mBlueVariance;
	bool mRepeatCycle;
	int mTotalParticles;
	bool mFadeParticles;
};

class Fireworks_B
{
public:
	Fireworks_B() 
	{
		INITIALIZE_MEMBERS(int, int);
	}
	bool mIsActive;
	bool mVaryColor;
	bool mRepeatCycle;
	bool mFadeParticles;
	char mRed;
	char mGreen;
	char mBlue;
	int mRedVariance;
	int mGreenVariance;
	int mBlueVariance;
	int mTotalParticles;
	int mOriginX;
	int mOriginY;
};

class Fireworks_C
{
public:
	Fireworks_C()
	{
		INITIALIZE_MEMBERS(int, int);
	}
	bool mIsActive:1;
	bool mVaryColor:1;
	bool mRepeatCycle:1;
	bool mFadeParticles:1;
	char mRed;
	char mGreen;
	char mBlue;
	int mRedVariance;
	int mGreenVariance;
	int mBlueVariance;
	int mTotalParticles;
	int mOriginX;
	int mOriginY;
};

class Fireworks_D
{
public:
	Fireworks_D()
	{
		INITIALIZE_MEMBERS(char, short);
	}
	bool mIsActive:1;
	bool mVaryColor:1;
	bool mRepeatCycle:1;
	bool mFadeParticles:1;
	char mRed;
	char mGreen;
	char mBlue;
	char mRedVariance;
	char mGreenVariance;
	char mBlueVariance;
	int mTotalParticles;
	short mOriginX;
	short mOriginY;
};

void AnalyzeStruct(const char *label, void *inptr, size_t bytes)
{
	cout << "sizeof(" << label << ") = ";
	cout << bytes << endl;

	const char *ptr = (const char *)inptr;
	for (size_t byte = 0; byte < bytes; ++byte) {
		char type = '.';
		switch (ptr[byte]) {
		case 0:
			type = '.';
			break;
		case 1:
			type = 'B';
			break;
		case 2:
			type = 'C';
			break;
		case 3:
			type = 'I';
			break;
		default:
			type = 'X';
			break;
		}
		cout << type;
	}
	cout << endl;
}

int main(int argc, char *argv[])
{
	Fireworks_A fireworksA;
	Fireworks_B fireworksB;
	Fireworks_C fireworksC;
	Fireworks_D fireworksD;

	// display the size and layout of each class
	AnalyzeStruct("Fireworks_A", &fireworksA, sizeof(fireworksA));
	AnalyzeStruct("Fireworks_B", &fireworksB, sizeof(fireworksB));
	AnalyzeStruct("Fireworks_C", &fireworksC, sizeof(fireworksC));
	AnalyzeStruct("Fireworks_D", &fireworksD, sizeof(fireworksD));

	return 0;
}
