/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   arglist.h
/// \author Martin Reddy
/// \brief  A container for flexibly-typed scalar values.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef ARGLIST_H
#define ARGLIST_H

#include <string>
#include <map>

namespace apibook {

class Arg
{
public:
	// constructor, destructor, copy constructor, and assignment
	Arg();
	~Arg();
	Arg(const Arg&);
	Arg &operator = (const Arg &other);

	// constructors to initialize with a value
	explicit Arg(bool value);
	explicit Arg(int value);
	explicit Arg(double value);
	explicit Arg(const std::string &value);

	// set the arg to be empty/undefined/NULL
	void Clear();

	// change the current value
	void Set(bool value);
	void Set(int value);
	void Set(double value);
	void Set(const std::string &value);

	// test the type of the current value
	bool Empty() const;
	bool ContainsBool() const;
	bool ContainsInt() const;
	bool ContainsDouble() const;
	bool ContainsString() const;

	// can the current value be returned as another type
	bool CanCovertToBool() const;
	bool CanCovertToInt() const;
	bool CanCovertToDouble() const;
	bool CanCovertToString() const;

	// return the current value as a specific type
	bool ToBool() const;
	int  ToInt() const;
	double ToDouble() const;
	std::string ToString() const;

public:
	class Base;
private:
	Base *mImpl;
};

typedef std::map<std::string, Arg> ArgDict;

class ArgList
{
public:
	ArgList();

	Arg Get(const std::string &name) const;
	size_t Size() const;
	ArgList &Add(const std::string &name, bool value);
	ArgList &Add(const std::string &name, int value);
	ArgList &Add(const std::string &name, double value);
	ArgList &Add(const std::string &name, const std::string &value);

private:
	std::map<std::string, Arg> mMap;
};

}

#endif
