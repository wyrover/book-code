#
# CMakeLists.txt - cmake build file for API Design examples
#
# Martin Reddy, 1 Jan 2010 - http://APIBook.com/
#

# Declare the minimum version of cmake that we need
cmake_minimum_required(VERSION 2.4)

# Define the include/link dirs
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../auto_timer")
link_directories("${CMAKE_CURRENT_BINARY_DIR}/../auto_timer")

# Build the executable
add_executable(virtual_speed main.cpp myclass.cpp)

# Link against the AutoTimer library
target_link_libraries(virtual_speed autotimer)
