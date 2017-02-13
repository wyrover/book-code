
set PATH=%~dp0;%PATH%
premake5 --file=vs2015-3rdparty.lua vs2015
premake5 --file=vs2015-3rdparty.lua vs2005
pause