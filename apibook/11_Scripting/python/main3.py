#!/usr/bin/python
#
# \file   main3.py
# \author Martin Reddy
# \brief  Demonstrate calling a C++ API from Python. 
#
# Copyright (c) 2010, Martin Reddy. All rights reserved.
# Distributed under the X11/MIT License. See LICENSE.txt.
# See http://APIBook.com/ for the latest version.
#

import phonebook

def person_str(self):
    return "Name: %s\nHome: %s" % (self.name, self.home_number)

phonebook.Person.__str__ = person_str

# add one contact
p = phonebook.Person()
p.name = 'Martin'
p.home_number = '(123) 456-7890'

print p

