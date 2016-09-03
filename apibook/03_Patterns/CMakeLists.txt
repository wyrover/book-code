#
# CMakeLists.txt - cmake build file for API Design examples
#
# Martin Reddy, 1 Jan 2010 - http://APIBook.com/
#

# Declare the minimum version of cmake that we need
cmake_minimum_required(VERSION 2.4)

# Specify the subdirectories to build
add_subdirectory(pimpl_bad)
add_subdirectory(pimpl_good)
add_subdirectory(pimpl_boost)
add_subdirectory(opaque_pointer)
add_subdirectory(singleton)
add_subdirectory(factory_simple)
add_subdirectory(factory_extensible)
add_subdirectory(proxy_simple)
add_subdirectory(proxy_interface)
add_subdirectory(adapter)
add_subdirectory(facade)
add_subdirectory(observer)
