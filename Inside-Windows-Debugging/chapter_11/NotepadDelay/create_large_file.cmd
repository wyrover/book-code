@echo off

setlocal

set FILE_SIZE=

:NEXTARG
if "%1" == "/?" goto :USAGE
if "%1" == "-?" goto :USAGE

if "%1" == "-size" set FILE_SIZE=%2&shift&shift&goto :NEXTARG

REM 
REM Check for unknown arguments and continue
REM
if "%1" NEQ "" goto :USAGE
if not defined FILE_SIZE goto :USAGE

set CMDLINE=fsutil.exe file createnew c:\temp\test.dat

if exist c:\temp goto :TMPDIREXISTS
md c:\temp

:TMPDIREXISTS
echo.
echo INFO: Invoking fsutil to create a %FILE_SIZE%-byte file...
echo CmdLine: ^(%CMDLINE%^ %FILE_SIZE%)
echo.
del c:\temp\test.dat
%CMDLINE% %FILE_SIZE%

:EXIT
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
echo USAGE: create_large_file.cmd [options] - create a large file for testing
echo. 
echo Options
echo        -size FileSizeInBytes
echo.

goto :EOF
