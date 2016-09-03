/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   currency.cpp
/// \author Martin Reddy
/// \brief  Demonstrating good C++ operator style.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "currency.h"

namespace apibook {

class Currency::Impl
{
public:
	int mValue;
};

// default constructor
Currency::Currency(int value) :
	mImpl(new Currency::Impl)
{
	mImpl->mValue = value;
}

Currency::~Currency()
{
	delete mImpl;
}

Currency::Currency(const Currency &obj) :
	mImpl(new Currency::Impl)
{
	mImpl->mValue = obj.mImpl->mValue;
}

int Currency::GetValue() const
{
	return mImpl->mValue;
}

Currency &Currency::operator =(const Currency &rhs)
{
	if (this != &rhs)
	{
		mImpl->mValue = rhs.mImpl->mValue;
	}
	return *this;
}

Currency &Currency::operator +=(const Currency &rhs)
{
	mImpl->mValue += rhs.mImpl->mValue;
	return *this;
}

Currency &Currency::operator -=(const Currency &rhs)
{
	mImpl->mValue -= rhs.mImpl->mValue;
	return *this;
}

Currency &Currency::operator *=(const Currency &rhs)
{
	mImpl->mValue *= rhs.mImpl->mValue;
	return *this;
}

Currency &Currency::operator /=(const Currency &rhs)
{
	mImpl->mValue /= rhs.mImpl->mValue;
	return *this;
}

Currency operator +(const Currency &lhs, const Currency &rhs)
{
	return Currency(lhs) += rhs;
}

Currency operator -(const Currency &lhs, const Currency &rhs)
{
	return Currency(lhs) -= rhs;
}

Currency operator *(const Currency &lhs, const Currency &rhs)
{
	return Currency(lhs) *= rhs;
}

Currency operator /(const Currency &lhs, const Currency &rhs)
{
	return Currency(lhs) /= rhs;
}

// post: return true iff lhs == rhs 
bool operator ==(const Currency &lhs, const Currency &rhs) 
{ 
	return lhs.GetValue() == rhs.GetValue();
} 

// post: return true iff lhs != rhs 
bool operator !=(const Currency &lhs, const Currency &rhs) 
{ 
	return ! (lhs == rhs); 
} 

// post: return true iff lhs < rhs 
bool operator <(const Currency &lhs, const Currency &rhs) 
{ 
	return lhs.GetValue() < rhs.GetValue();
}

// post: return true iff lhs > rhs 
bool operator >(const Currency &lhs, const Currency &rhs) 
{ 
	return rhs < lhs; 
} 

// post: return true iff lhs <= rhs 
bool operator <=(const Currency &lhs, const Currency &rhs) 
{ 
	return ! (lhs > rhs); 
} 

// post: return true iff lhs >= rhs 
bool operator >=(const Currency &lhs, const Currency &rhs) 
{ 
	return rhs <= lhs; 
}

std::ostream& operator <<(std::ostream &os, const Currency &obj)
{
	os << obj.GetValue() << " units";
	return os;
}

}
