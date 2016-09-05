#!/usr/bin/python
#
# \file   main.py
# \author Martin Reddy
# \brief  Demonstrate calling a C++ API from Python. 
#
# Copyright (c) 2010, Martin Reddy. All rights reserved.
# Distributed under the X11/MIT License. See LICENSE.txt.
# See http://APIBook.com/ for the latest version.
#

import phonebook

book = phonebook.PhoneBook()

p = phonebook.Person()
p.name = 'Martin'
p.home_number = '(123) 456-7890'
book.add_person(p)

p = phonebook.PersonWithCell()
p.name = 'Genevieve'
p.home_number = '(123) 456-7890'
p.cell_number = '(213) 097-2134'
book.add_person(p)

def better_person_print(self):
    return "Name = " + self.name
phonebook.PersonWithCell.__str__ = better_person_print

p = phonebook.PersonWithCell()
p.name = 'Frances'
p.home_number = '(123) 456-7890'
p.cell_number = '(213) 097-2134'
book.add_person(p)

class PyPersonWithCell(phonebook.Person):
    def __str__(self):
        return "%s: %s (Home), %s (Cell)" % (self.name, self.home_number, self.cell_number)
    def get_cell_number(self):
        return self.cell
    def set_cell_number(self, n):
        self.cell = n
    cell_number = property(get_cell_number, set_cell_number)

print "REMOVE"
book.remove_person('Frances')        

p = PyPersonWithCell()
p.name = 'Elsie'
p.home_number = '(123) 456-7890'
p.cell_number = '(213) 097-2134'
book.add_person(p)
print p

print "No. of contacts =", book.size()
for person in book:
    print "PERSON",person
