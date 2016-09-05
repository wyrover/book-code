#
# CMakeLists.txt - cmake build file for API Design examples
#
# Martin Reddy, 1 Jan 2010 - http://APIBook.com/
#

# Declare the minimum version of cmake that we need
cmake_minimum_required(VERSION 2.4)

# Setup the name for this CMake project
project(APIBook)

# add our external include path (for Boost headers)
include_directories("${CMAKE_SOURCE_DIR}/external")

# compile with debug information and all warnings by default
if (CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS "-g -Wall")
endif ()

# Specify the subdirectories to build
add_subdirectory(02_Qualities)
add_subdirectory(03_Patterns)
add_subdirectory(04_Design)
add_subdirectory(05_Styles)
add_subdirectory(06_Cpp_Style)
add_subdirectory(07_Performance)
add_subdirectory(08_Versioning)
add_subdirectory(09_Documentation)
add_subdirectory(10_Testing)
add_subdirectory(11_Scripting)
add_subdirectory(12_Extensibility)
add_subdirectory(AA_Libraries)

# Note, some subdirs are not built because they have additional
# dependencies - see their CMakeLists.txt files for details.
