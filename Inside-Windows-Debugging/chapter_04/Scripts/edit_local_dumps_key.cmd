@echo off

setlocal

set DUMP_FOLDER=
set DUMP_TYPE=

:NEXTARG
if "%1" == "/?" goto :USAGE
if "%1" == "-?" goto :USAGE

if "%1" == "-df" set DUMP_FOLDER=%2&shift&shift&goto :NEXTARG
if "%1" == "-dt" set DUMP_TYPE=%2&shift&shift&goto :NEXTARG

REM 
REM Check for unknown arguments and continue
REM
if "%1" NEQ "" goto :USAGE

if not defined DUMP_FOLDER (
    goto :USAGE
)
if not defined DUMP_TYPE (
    goto :USAGE
)

echo.
echo INFO: Adding LocalDumps Registry Values...
echo.

reg.exe add "HKLM\Software\Microsoft\Windows\Windows Error Reporting\LocalDumps" /f
reg.exe add "HKLM\Software\Microsoft\Windows\Windows Error Reporting\LocalDumps" /v DumpType /t REG_DWORD /d %DUMP_TYPE% /f
reg.exe add "HKLM\Software\Microsoft\Windows\Windows Error Reporting\LocalDumps" /v DumpFolder /t REG_EXPAND_SZ /d %DUMP_FOLDER% /f

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
echo USAGE: edit_local_dumps_key.cmd [options] - edit local dumps settings
echo. 
echo Options
echo        -df - folder location where automatic crash dumps are to be saved
echo        -dt - type of dump to generate (1: minidump, 2: full dump)
echo.

goto :EOF
