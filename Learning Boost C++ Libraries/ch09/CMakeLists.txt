cmake_minimum_required(VERSION 2.6)

project("LearnBoostAsio")

# Glob all .cpp files
file(GLOB CPP_SRCS *.cpp)

foreach (cpp_file IN LISTS CPP_SRCS)
  get_filename_component (exe_name ${cpp_file} NAME_WE)

  # Create an executable
  add_executable(${exe_name} ${cpp_file})

if (UNIX)
  # Link the libraries Foo and Bar to the executable.
  # Shared libraries preferred over static libraries when both present.
  # Libraries looked up in directories specified using link_directories. 
  target_link_libraries(${exe_name} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_IOSTREAMS_LIBRARY} ${Boost_REGEX_LIBRARY})
endif()
endforeach()

