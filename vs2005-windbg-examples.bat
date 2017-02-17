cd /d "%~dp0"
set PATH=%~dp0;%PATH%
premake5 --file=vs2005-windbg-examples.lua vs2015
premake5 --file=vs2005-windbg-examples.lua vs2005
