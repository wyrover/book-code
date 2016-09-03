/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   phonebook_wrap.cpp
/// \author Martin Reddy
/// \brief  A boost::python wrapper for PhoneBook
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "phonebook.h"

#include <boost/python.hpp>

#include <sstream>
#include <iostream>

using namespace boost::python;

std::ostream &operator<<(std::ostream &os, const Person &p)
{
	os << p.GetName() << ": " << p.GetHomeNumber();
	return os;
}

static std::string PrintPersonWithCell(const PersonWithCell *p)
{
	std::ostringstream stream;
	stream << "Name: " << p->GetName() << ", Home: ";
	stream << p->GetHomeNumber() << ", Cell: ";
	stream << p->GetCellNumber();
	return stream.str();
}

struct convert_person_ptr
{
	static PyObject* convert(Person * const &p)
	{
		return boost::python::incref(boost::python::object(*p).ptr());
	}
};


BOOST_PYTHON_MODULE(phonebook)
{
	class_<Person>("Person", init<>())
		.def(init<std::string>())
		.add_property("name", &Person::GetName, &Person::SetName)
		.add_property("home_number", &Person::GetHomeNumber, &Person::SetHomeNumber)
		.def(self_ns::str(self))
		;

	class_<PersonWithCell, bases<Person> >("PersonWithCell")
		.def(init<std::string>())
		.add_property("cell_number", &PersonWithCell::GetCellNumber, &PersonWithCell::SetCellNumber)
		.def("__str__", &PrintPersonWithCell)
		;

	class_<PhoneBook>("PhoneBook")
		.def("size", &PhoneBook::GetSize)
		.def("add_person", &PhoneBook::AddPerson)
		.def("remove_person", &PhoneBook::RemovePerson)
		.def("find_person", &PhoneBook::FindPerson, return_value_policy<reference_existing_object>())
		.def("__iter__", range(&PhoneBook::begin, &PhoneBook::end));
		;

	to_python_converter<Person *, convert_person_ptr>();
}

