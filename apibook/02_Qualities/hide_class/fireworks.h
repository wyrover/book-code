/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   fireworks.h
/// \author Martin Reddy
/// \brief  An illustration of using private classes.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef FIREWORKS_H
#define FIREWORKS_H

#include <vector>

namespace apibook {

///
/// A simple fireworks particle system, used to demonstrate
/// the use of private classes to hide implementation state.
///
class Fireworks
{
public:
	Fireworks();

	/// Set the (x, y) origin of the fireworks effect
	void SetOrigin(double x, double y);
	/// Set the RGB color (0..1) for each particle
	void SetColor(float r, float g, float b);
	/// Set the gravity acting on each particle (meters/sec)
	void SetGravity(float g);
	/// Set the speed of the particle simulation
	void SetSpeed(float s);
	/// Set the number of particles in the simulation
	void SetNumberOfParticles(int num);

	/// Start (or continue) the simulation
	void Start();
	/// Stop the simulation
	void Stop();
	/// Advance the simulation by dt seconds 
	void NextFrame(float dt);

private:
	// FireParticle represents internal hidden state
	// (You could also forward declare this class and
	// only provide the definition in the .cpp file.)
	class FireParticle
	{
	public:
		double mX, mY;
		double mVelocityX, mVelocityY;
		double mAccelerationX, mAccelerationY;
		double mLifeTime;
	};

	double mOriginX, mOriginY;
	float mRed, mGreen, mBlue;
	float mGravity;
	float mSpeed;
	bool mIsActive;
	std::vector<FireParticle *> mParticles;
};

}

#endif
