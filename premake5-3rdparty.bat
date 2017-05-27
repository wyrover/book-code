
set PATH=%~dp0;%PATH%
::premake5 --file=premake5-3rdparty.lua vs2005
premake5 --file=premake5-3rdparty.lua vs2013
pause