@echo off

setlocal

set XPERF_CMDLINE=xperf.exe -stop -stop UserSession -d c:\temp\merged.etl

if exist c:\temp goto :TMPDIREXISTS
md c:\temp

:TMPDIREXISTS
echo.
echo INFO: Invoking xperf to stop the kernel and user sessions...
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
