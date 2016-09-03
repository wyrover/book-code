#
# CMakeLists.txt - cmake build file for API Design examples
#
# Martin Reddy, 1 Jan 2010 - http://APIBook.com/
#

# Declare the minimum version of cmake that we need
cmake_minimum_required(VERSION 2.4)

# N.B. you need to install the GMock run-time in this
# directory under a "gmock" subdir for this to work.
include_directories(./gmock/include)
link_directories(./gmock/lib)

# create the libraries
add_executable(test_stub test_stub.cpp wargame.cpp)

add_executable(test_mock test_mock.cpp wargame.cpp)
target_link_libraries(test_mock gmock gtest)
