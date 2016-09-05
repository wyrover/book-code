#
# CMakeLists.txt - cmake build file for API Design examples
#
# Martin Reddy, 1 Jan 2010 - http://APIBook.com/
#

# Declare the minimum version of cmake that we need
cmake_minimum_required(VERSION 2.4)

# Specify the subdirectories to build
add_subdirectory(constructors)
add_subdirectory(operators)
add_subdirectory(template_explicit)
add_subdirectory(template_polymorphism)
add_subdirectory(template_specialization)
add_subdirectory(bad_friends)
add_subdirectory(extern_leakage)
