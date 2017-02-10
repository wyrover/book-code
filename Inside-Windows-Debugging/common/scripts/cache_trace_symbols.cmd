set _NT_SYMBOL_PATH=srv*c:\localsymbolcache*http://msdl.microsoft.com/download/symbols
set _NT_SYMCACHE_PATH=c:\symcache

@echo off

setlocal

set XPERF_CMDLINE=xperf -i c:\temp\merged.etl -symbols -a symcache -build

echo.
echo INFO: Invoking xperf to cache trace symbols locally...
echo CmdLine: ^(%XPERF_CMDLINE%^)
echo.
%XPERF_CMDLINE%

if %ERRORLEVEL% NEQ 0 (
    echo. 
    echo FAILED!
    echo.
) else (
    echo. 
    echo SUCCESS. 
    echo.
)
