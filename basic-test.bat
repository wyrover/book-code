cd /d "%~dp0"
set PATH=%~dp0;%PATH%
premake5 --file=basic-test.lua vs2015
premake5 --file=basic-test.lua vs2005
