/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   arglist.cpp
/// \author Martin Reddy
/// \brief  A container for flexibly-typed scalar values.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "arglist.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

namespace apibook {

class Arg::Base
{
public:
	enum Type {
		INTEGER,
		BOOLEAN,
		DOUBLE,
		STRING
	};

	virtual ~Base() {}
	virtual Arg::Base *Clone() const = 0;
	virtual Type GetType() const = 0;

	virtual bool CanConvertToBool() const = 0;
	virtual bool CanConvertToInt() const = 0;
	virtual bool CanConvertToDouble() const = 0;
	virtual bool CanConvertToString() const = 0;

	virtual bool ToBool() const = 0;
	virtual int  ToInt() const = 0;
	virtual double ToDouble() const = 0;
	virtual std::string ToString() const = 0;
};

class ArgBool : public Arg::Base
{
public:
	ArgBool(bool value) : mValue(value) {}
	ArgBool *Clone() const { return new ArgBool(mValue); }

	Type GetType() const { return BOOLEAN; }

	bool CanConvertToBool() const { return true; }
	bool CanConvertToInt() const { return true; }
	bool CanConvertToDouble() const { return true; }
	bool CanConvertToString() const { return true; }

	bool ToBool() const { return mValue; }
	int  ToInt() const { return mValue ? 0 : 1; }
	double ToDouble() const { return mValue ? 0.0 : 1.0; }
	std::string ToString() const { return mValue ? "true" : "false"; }

private:
	bool mValue;
};

class ArgInt : public Arg::Base
{
public:
	ArgInt(int value) : mValue(value) {}
	ArgInt *Clone() const { return new ArgInt(mValue); }
  
	Type GetType() const { return INTEGER; }

	bool CanConvertToBool() const { return true; }
	bool CanConvertToInt() const { return true; }
	bool CanConvertToDouble() const { return true; }
	bool CanConvertToString() const { return true; }

	bool ToBool() const { return (mValue != 0); }
	int  ToInt() const { return mValue; }
	double ToDouble() const { return static_cast<double>(mValue); }
	std::string ToString() const
	{
		char buffer[256];
		sprintf(buffer, "%d", mValue);
		return std::string(buffer);
	}

private:
	int mValue;
};

class ArgDouble : public Arg::Base
{
public:
	ArgDouble(double value) : mValue(value) {}
	ArgDouble *Clone() const { return new ArgDouble(mValue); }

	Type GetType() const { return DOUBLE; }

	bool CanConvertToBool() const { return true; }
	bool CanConvertToInt() const { return true; }
	bool CanConvertToDouble() const { return true; }
	bool CanConvertToString() const { return true; }

	bool ToBool() const { return (mValue != 0.0); }
	int  ToInt() const { return static_cast<int>(mValue); }
	double ToDouble() const { return mValue; }
	std::string ToString() const
	{
		char buffer[256];
		sprintf(buffer, "%f", mValue);
		return std::string(buffer);
	}

private:
	double mValue;
};

class ArgString : public Arg::Base
{
public:
	ArgString(const std::string &value) : mValue(value) {}
	ArgString *Clone() const { return new ArgString(mValue); }

	Type GetType() const { return STRING; }

	bool CanConvertToBool() const
	{
		return (mValue == "true" || mValue == "1" ||
		        mValue == "false" || mValue == "0");
	}
	bool CanConvertToInt() const
	{
		const char *cstr = mValue.c_str();
		char *result = 0;
		long d = strtol(cstr, &result, 10);
		(void) d;
		return result != cstr;
	}
	bool CanConvertToDouble() const
	{
		const char *cstr = mValue.c_str();
		char *result = 0;
		double d = strtod(cstr, &result);
		(void) d;
		return result != cstr;
	}
	bool CanConvertToString() const { return true; }

	bool ToBool() const
	{
		return (mValue == "true" || mValue == "1");
	}
	int  ToInt() const
	{
		char *result = 0;
		return (int)strtol(mValue.c_str(), &result, 10);
	}
	double ToDouble() const
	{
		char *result = 0;
		return strtod(mValue.c_str(), &result);
	}
	std::string ToString() const { return mValue; }

private:
	std::string mValue;
};

Arg::Arg() :
	mImpl(NULL)
{
}

Arg::~Arg()
{
	delete mImpl;
	mImpl = NULL;
}

Arg::Arg(const Arg& other) :
	mImpl(other.mImpl ? other.mImpl->Clone() : NULL)
{
}

Arg &Arg::operator = (const Arg &other)
{
	Arg::Base *newval = other.mImpl ? other.mImpl->Clone() : NULL;
	delete mImpl;
	mImpl = newval;
	return *this;
}

Arg::Arg(bool value) :
	mImpl(new ArgBool(value))
{
}

Arg::Arg(int value) :
	mImpl(new ArgInt(value))
{
}

Arg::Arg(double value) :
	mImpl(new ArgDouble(value))
{
}

Arg::Arg(const std::string &value) :
	mImpl(new ArgString(value))
{
}

void Arg::Clear()
{
	delete mImpl;
	mImpl = NULL;
}

void Arg::Set(bool value) 
{
	delete mImpl;
	mImpl = new ArgBool(value);
}

void Arg::Set(int value)
{
	delete mImpl;
	mImpl = new ArgInt(value);
}

void Arg::Set(double value)
{
	delete mImpl;
	mImpl = new ArgDouble(value);
}

void Arg::Set(const std::string &value)
{
	delete mImpl;
	mImpl = new ArgString(value);
}

bool Arg::Empty() const { return (mImpl == NULL); }
bool Arg::ContainsBool() const { return (mImpl && mImpl->GetType() == Arg::Base::BOOLEAN); }
bool Arg::ContainsInt() const { return (mImpl && mImpl->GetType() == Arg::Base::INTEGER); }
bool Arg::ContainsDouble() const { return (mImpl && mImpl->GetType() == Arg::Base::DOUBLE); }
bool Arg::ContainsString() const { return (mImpl && mImpl->GetType() == Arg::Base::STRING); }

bool Arg::CanCovertToBool() const { return (mImpl && mImpl->CanConvertToBool()); }
bool Arg::CanCovertToInt() const { return (mImpl && mImpl->CanConvertToInt()); }
bool Arg::CanCovertToDouble() const { return (mImpl && mImpl->CanConvertToDouble()); }
bool Arg::CanCovertToString() const { return (mImpl && mImpl->CanConvertToString()); }

bool Arg::ToBool() const
{
	if (! mImpl) return false;
	return mImpl->ToBool();
}

int Arg:: ToInt() const
{
	if (! mImpl) return 0;
	return mImpl->ToInt();
}

double Arg::ToDouble() const
{
	if (! mImpl) return 0.0;
	return mImpl->ToDouble();
}

std::string Arg::ToString() const
{
	if (! mImpl) return "";
	return mImpl->ToString();
}

ArgList::ArgList()
{
}

Arg ArgList::Get(const std::string &name) const
{
	std::map<std::string, Arg>::const_iterator it = mMap.find(name);
	if (it == mMap.end())
	{
		return Arg();
	}
	return it->second;
}

size_t ArgList::Size() const
{
	return mMap.size();
}

ArgList &ArgList::Add(const std::string &name, bool value)
{
	mMap[name] = Arg(value);
	return *this;
}

ArgList &ArgList::Add(const std::string &name, int value)
{
	mMap[name] = Arg(value);
	return *this;
}

ArgList &ArgList::Add(const std::string &name, double value)
{
	mMap[name] = Arg(value);
	return *this;
}

ArgList &ArgList::Add(const std::string &name, const std::string &value)
{
	mMap[name] = Arg(value);
	return *this;
}

}
