#
# CMakeLists.txt - cmake build file for API Design examples
#
# Martin Reddy, 1 Jan 2010 - http://APIBook.com/
#

# Declare the minimum version of cmake that we need
cmake_minimum_required(VERSION 2.4)

# These directories only build under GCC
if (CMAKE_COMPILER_IS_GNUCC)
  add_subdirectory(gcc_entry_point)
  add_subdirectory(gcc_plugin)
endif (CMAKE_COMPILER_IS_GNUCC)
