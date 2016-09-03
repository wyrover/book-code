#
# CMakeLists.txt - cmake build file for API Design examples
#
# Martin Reddy, 1 Jan 2010 - http://APIBook.com/
#

# Declare the minimum version of cmake that we need
cmake_minimum_required(VERSION 2.4)

# create the library
add_library(library SHARED library.cpp)

add_executable(gcc_entry_point main.cpp)
target_link_libraries(gcc_entry_point library)
