set _NT_SYMBOL_PATH=srv*c:\localsymbolcache*http://msdl.microsoft.com/download/symbols
set _NT_SYMCACHE_PATH=c:\symcache

@echo off

setlocal

set ADDITIONAL_SYMBOL_PATHS=

:NEXTARG
if "%1" == "/?" goto :USAGE
if "%1" == "-?" goto :USAGE

if "%1" == "-sympath" set ADDITIONAL_SYMBOL_PATHS=%2&shift&shift&goto :NEXTARG

REM 
REM Check for unknown arguments and continue
REM
if "%1" NEQ "" goto :USAGE

if defined ADDITIONAL_SYMBOL_PATHS (
    endlocal
    echo.
    echo set _NT_SYMBOL_PATH=%_NT_SYMBOL_PATH%;%ADDITIONAL_SYMBOL_PATHS%
    echo.
    set _NT_SYMBOL_PATH=%_NT_SYMBOL_PATH%;%ADDITIONAL_SYMBOL_PATHS%
    setlocal
)

set XPERF_CMDLINE=xperf.exe c:\temp\merged.etl
echo.
echo INFO: Invoking xperf to view the trace...
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

goto :EOF

:USAGE
echo.
echo USAGE: view_trace.cmd [options] - view the the collected trace file
echo. 
echo Options
echo        -sympath - additional (optional) symbols path for your code
echo.

goto :EOF
