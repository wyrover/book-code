/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   phonebook.h
/// \author Martin Reddy
/// \brief  A simple phone book API
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include <string>
#include <vector>

///
/// A single contact in the phone book
///
class Person
{
public:
	Person();
	Person(const std::string &name);
	virtual ~Person();

	/// Set the name for this contact
	void SetName(const std::string &name);
	/// Return the name for this contact
	std::string GetName() const;

	/// Set the home phone number for this contact
	void SetHomeNumber(const std::string &number);
	/// Return the home phone number for this contact
	std::string GetHomeNumber() const;

	/// Return a copy of this object
	virtual Person *clone() const;

private:
	std::string mName;
	std::string mHomeNumber;
};

///
/// A single contact in the phone book, with
/// the ability to include a cell phone number
///
class PersonWithCell : public Person
{
public:
	PersonWithCell();
	PersonWithCell(const std::string &name);

	/// Set the cell phone number for the contact
	void SetCellNumber(const std::string &number);
	/// Return the cell phone number for the contact
	std::string GetCellNumber() const;

	/// Return a copy of this object
	virtual PersonWithCell *clone() const;

private:
	std::string mCellNumber;
};

///
/// A phone book that can contain multiple contacts
///
class PhoneBook
{
public:
	PhoneBook();
	~PhoneBook();

	/// Return the number of contacts in the phone book
	int GetSize() const;

	/// Add a contact to the phone book
	void AddPerson(Person *p);
	/// Remove a contact from the phone book
	void RemovePerson(const std::string &name);
	/// Find a contact in the phone book, or return NULL
	Person *FindPerson(const std::string &name);

	typedef std::vector<Person *> PersonList;
	PersonList::iterator begin();
	PersonList::iterator end();

private:
	PersonList mList;
};

#endif
