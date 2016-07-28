# Minimum version of CMake required is 2.6
cmake_minimum_required(VERSION 2.6)

# Name of the top-level project / solution
#  is LearnBoostAsio
project("LearnBoost")

# print an informational message
message("Building for ${CMAKE_SYSTEM_NAME}.")

# Build rule changes do not trigger rebuild
set (CMAKE_SKIP_RULE_DEPENDENCY TRUE)

# During build, print build command lines
set (CMAKE_VERBOSE_MAKEFILE TRUE)

# Define a custom variable
set(MY_PLATFORM_DIR ${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_VERSION}/${CMAKE_HOST_SYSTEM_PROCESSOR})

# Set the directory where executables
#   are created
set(RUNTIME_OUTPUT_DIRECTORY
   ${CMAKE_SOURCE_DIR}/bin/${MY_PLATFORM_DIR})
set(EXECUTABLE_OUTPUT_PATH
   ${CMAKE_SOURCE_DIR}/bin/${MY_PLATFORM_DIR})

# Set the directory where shared libraries
#   are created
set(LIBRARY_OUTPUT_PATH
   ${CMAKE_SOURCE_DIR}/lib/${MY_PLATFORM_DIR})

# Set the directory where static libraries (archives)
#   are created
set(ARCHIVE_OUTPUT_PATH
   ${CMAKE_SOURCE_DIR}/lib/${MY_PLATFORM_DIR})

# Set preprocessor definitions
# Note how env var BUILD_DATE is read using $ENV{BUILD_DATE}
if (WIN32)
add_definitions(-DWIN32 -DWINNT -DBOOST_ALL_DYN_LINK -DBOOST_AUTO_LINK_TAGGED -D_WIN32_WINNT=0x0501)
else ()
add_definitions(-D_REENTRANT)
endif ()

# Set additional compiler flags
#  for the C++ and C compilers
if (WIN32)
set(CMAKE_CXX_FLAGS "/EHsc /Zi" CACHE STRING "C++ flags" FORCE)
else ()
set(CMAKE_CXX_FLAGS "-g -pthread -std=c++11" CACHE STRING "C++ flags" FORCE)
endif()

# Set linker flags
if (WIN32)
set(CMAKE_EXE_LINKER_FLAGS, "/SAFESEH /IGNORE:4221 /INCREMENTAL:NO")
endif()

set(BOOST_ROOT $ENV{BOOST_DIR})
find_package(Boost 1.56 COMPONENTS system thread filesystem date_time chrono iostreams regex coroutine context date_time timer REQUIRED)

if(Boost_FOUND)
  # Set of directories where the preprocessor
  #  looks for include files
  include_directories(${Boost_INCLUDE_DIR})

  # Set of directories where the linker
  #  looks for libraries (shared or static)
  link_directories(${Boost_LIBRARY_DIR})
endif()

# Set linker language to C, useful when linking
#  a mix of C and C++ libraries into a single binary
set(LINKER_LANGUAGE C)

# Recurse into the following subdirectories
add_subdirectory(ap01)
add_subdirectory(ch02)
add_subdirectory(ch03)
add_subdirectory(ch04)
add_subdirectory(ch05)
add_subdirectory(ch06)
add_subdirectory(ch08)
add_subdirectory(ch09)
add_subdirectory(ch10)
add_subdirectory(ch11)


