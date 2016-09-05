///
/// \file   phonebook.i
/// \brief  SWIG interface file for the phone book API
/// \author Martin Reddy
/// \detail Example code from the book "API Design for C++".
///         See http://APIDesign.com/ for the latest version.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
///

%module(directors="1") phonebook
%{

#include "phonebook.h"
#include <sstream>
#include <iostream>

%}

%include "std_string.i"
%include "std_vector.i"

%feature("director");

%rename("name") Person::GetName;
%rename("name=") Person::SetName;

%rename("home_number") Person::GetHomeNumber;
%rename("home_number=") Person::SetHomeNumber;

class Person
{
public:
	Person();
	Person(const std::string &name);
	virtual ~Person();

	void SetName(const std::string &name);
	std::string GetName() const;

	void SetHomeNumber(const std::string &number);
	std::string GetHomeNumber() const;

	%extend {
		std::string to_s() {
			std::ostringstream stream;
			stream << self->GetName() << ": ";
		stream << self->GetHomeNumber();
			return stream.str();
		}
	}
};

%rename("cell_number") PersonWithCell::GetCellNumber;
%rename("cell_number=") PersonWithCell::SetCellNumber;

class PersonWithCell : public Person
{
public:
	PersonWithCell();
	PersonWithCell(const std::string &name);

	void SetCellNumber(const std::string &number);
	std::string GetCellNumber() const;
};

%rename("empty?") PhoneBook::IsEmpty;
%rename("size") PhoneBook::GetSize;
%rename("add_person") PhoneBook::AddPerson;
%rename("remove_person") PhoneBook::RemovePerson;
%rename("find_person") PhoneBook::FindPerson;

class PhoneBook
{
public:
	bool IsEmpty() const;
	int GetSize() const;

	void AddPerson(Person *p);
	void RemovePerson(const std::string &name);
	Person *FindPerson(const std::string &name);
};
