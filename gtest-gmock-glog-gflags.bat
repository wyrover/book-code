cd /d "%~dp0"
set PATH=%~dp0;%PATH%
premake5 --file=gtest-gmock-glog-gflags.lua vs2015
::premake5 --file=projects.lua vs2005
pause