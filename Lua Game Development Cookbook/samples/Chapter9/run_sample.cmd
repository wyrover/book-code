@echo off

if %PROCESSOR_ARCHITECTURE%==x86 (
	set ARCHITECTURE=x86
) ELSE (
	if %PROCESSOR_ARCHITECTURE%==AMD64 (
		set ARCHITECTURE=x86_64
	)
)

set ROOT_PREFIX=..\..\common
set BIN_PATH=%ROOT_PREFIX%\lib\external\platform-specific\Windows\%ARCHITECTURE%
set LUA_INTERPRETER=%BIN_PATH%\lua5.1.exe
set LUA_INIT=__ROOT_PREFIX = os.getenv('ROOT_PREFIX') or '../../common';local core = dofile(__ROOT_PREFIX..'/lib/core.lua')
set PATH=.;%BIN_PATH%
%LUA_INTERPRETER% %*
