#!/usr/bin/python
#
# \file   main2.py
# \author Martin Reddy
# \brief  Demonstrate calling a C++ API from Python. 
#
# Copyright (c) 2010, Martin Reddy. All rights reserved.
# Distributed under the X11/MIT License. See LICENSE.txt.
# See http://APIBook.com/ for the latest version.
#

import phonebook

# create the phonebook
book = phonebook.PhoneBook()

# add one contact
p = phonebook.Person()
p.name = 'Martin'
p.home_number = '(123) 456-7890'
book.add_person(p)

# add another contact
p = phonebook.Person()
p.name = 'Genevieve'
p.home_number = '(123) 456-7890'
book.add_person(p)

# display number of contacts added
print "No. of contacts =", book.size()

