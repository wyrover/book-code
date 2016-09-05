#
# CMakeLists.txt - cmake build file for API Design examples
#
# Martin Reddy, 1 Jan 2010 - http://APIBook.com/
#

# Declare the minimum version of cmake that we need
cmake_minimum_required(VERSION 2.4)

#
# Prerequisistes:
#
# To build this example, you need to download and build
# the Ruby source code from http://www.ruby-lang.org/
#
# You also need to install SWIG from http//www.swig.org/
# and ensure that the "swig" command is in your path.
#
# Building:
#
# 1. Set RUBY_SRC_DIR (below) to the directory where
#    you installed and built the ruby sources.
# 2. Use CMake to configure as usual and do a "make" in
#    the build dir to build the example module.
# 3. Do a "make run" in the build dir to run the example
#    Ruby script that accesses the C++ API.
#

# Set this to your ruby source directory
set(RUBY_SRC_DIR "/Users/reddy/src/ruby-1.8.7")

# try to find the ruby binary in your source dir
find_program(RUBY_BIN ruby ${RUBY_SRC_DIR})
if (RUBY_BIN STREQUAL "RUBY_BIN-NOTFOUND")
  message(FATAL_ERROR "Cannot find ruby binary. Have you built the Ruby sources?")
endif ()

# try to find the SWIG command in your path
find_program(SWIG_BIN swig)
if (SWIG_BIN STREQUAL "SWIG_BIN-NOTFOUND")
  message(FATAL_ERROR "Cannot find SWIG binary. Have you installed SWIG?")
endif ()

# we let the ruby mkmf package do the build for us,
# in a isolated "install" dir
set(INSTALL_DIR "${CMAKE_BINARY_DIR}/install")

# create the various files that 'mkmf' needs in the install dir
execute_process(
  COMMAND "${CMAKE_COMMAND}" -E make_directory "${INSTALL_DIR}"
  COMMAND "${SWIG_BIN}" -o "${INSTALL_DIR}/phonebook_wrap.cxx" -c++ -ruby "${CMAKE_SOURCE_DIR}/phonebook.i"
  COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/main.rb" "${INSTALL_DIR}"
)

# create the build command: a script which uses 'mkmf' to create
# a Makefile in the install directory and run that script
add_custom_target(install ALL
  COMMAND "${RUBY_BIN}" "-I${RUBY_SRC_DIR}" "-I${RUBY_SRC_DIR}/lib" "${CMAKE_SOURCE_DIR}/extconf.rb"
  COMMAND make
  WORKING_DIRECTORY ${INSTALL_DIR}
)

# add a convenience 'run' target to run the ruby script
add_custom_target(run
  COMMAND "${RUBY_BIN}" "${INSTALL_DIR}/main.rb"
  WORKING_DIRECTORY ${INSTALL_DIR}
)
