Building code examples
------------------------------------
Here are the instructions for building the code examples:

Prerequisites:
1. You must have CMake 2.8.10 or later installed on your Windows or Linux box.
2. You must have Visual Studio 2013 or later on Windows.
3. You must have g++ 4.8.1 or later on Linux.
4. The current scripts only work with Boost 1.56. If you want to use a
more recent version, open lbcpp-src/CMakeLists.txt - look for the
string 1.56 and replace it with 1.57 or 1.58 and rerun from step 2
below.

Steps on Linux:
1. Extract the attached source archive (lbcpp-src.zip), then:
cd lbcpp-src

2. If you did not install Boost OS native package but built it from
source, then:
export BOOST_DIR=<dir_containing_include_and_lib_directory_of_boost>

3. Create a cmake_bin directory and generate the build system inside it.
mkdir -p cmake_bin
cd cmake_bin
cmake ..

4. Run the build:
make

5. Locate the binaries:
cd ../bin/Linux
cd <os_and_version_specific_directories>


Steps on Windows with Visual Studio:
1. Extract the attached source archive using your 7-zip or a similar tool.

2. In cmd.exe:
cd lbcpp-src
set BOOST_DIR=<dir_containing_include_and_lib_directory_of_boost>

3. Create a cmake_bin directory and generate the build system. In cmd.exe:
md cmake_bin
cd cmake_bin
cmake -G "Visual Studio 12" ..

Use the appropriate generator string for your version of Visual
Studio. Visual Studio 12 is for 32-bit Visual Studio 2013. If you want
to use 64-bit VS 2013, then use "Visual Studio 2013 Win64" instead.

4. Run the build:
Open the solution file generated at cmake_bin\LearnBoost.sln, in Visual Studio.
Right click on each project and choose build to build that project, or
choose to build the "ALL BUILDS" project to build everything.

5. Locate the binaries:
cd ..\bin\Windows
cd <os_and_version_specific_directories>
