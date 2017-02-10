@echo off

setlocal

set XPERF_CMDLINE=xperf.exe -i c:\temp\merged.etl -o c:\temp\stats.txt -a tracestats -detail

echo.
echo INFO: Invoking xperf to dump provider statistics in the trace file...
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
