#!/usr/bin/ruby
#
# \file   main.rb
# \author Martin Reddy
# \brief  Demonstrate calling a C++ API from Ruby.
#
# Copyright (c) 2010, Martin Reddy. All rights reserved.
# Distributed under the X11/MIT License. See LICENSE.txt.
# See http://APIBook.com/ for the latest version.
#

require 'phonebook'

book = Phonebook::PhoneBook.new

p = Phonebook::Person.new
p.name = 'Martin'
p.home_number = '(123) 456-7890'
book.add_person(p)

p = Phonebook::Person.new('Genevieve')
p.home_number = '(123) 456-7890'
book.add_person(p)

class RbPersonWithCell < Phonebook::Person
    def initialize
        puts "RbPersonWithCell"
        super()
    end
    def cell_number
        @cell
    end
    def cell_number=(number)
        @cell = number
    end
    def to_s()
        "#{name}: #{home_number} / #{@cell}"
    end
end

p = RbPersonWithCell.new
p.name = 'Foo'
p.home_number = '42'
p.cell_number = '101'
puts p
book.add_person(p)

puts "No. of contacts: #{book.size}"

puts book.find_person('Martin')
puts book.find_person('Genevieve')
puts book.find_person('Foo')

