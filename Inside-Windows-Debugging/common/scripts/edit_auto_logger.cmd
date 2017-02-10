@echo off

setlocal

set ADDITIONAL_USERPROVIDER_STACK=
set ADDITIONAL_USERPROVIDER_GUID=
set DISABLE_USER_SESSION=

:NEXTARG
if "%1" == "/?" goto :USAGE
if "%1" == "-?" goto :USAGE

if "%1" == "-enable" set ADDITIONAL_USERPROVIDER_GUID=%2&shift&shift&goto :NEXTARG
if "%1" == "-stack" set ADDITIONAL_USERPROVIDER_STACK=true&shift&goto :NEXTARG
if "%1" == "-disable" set DISABLE_USER_SESSION=true&shift&goto :NEXTARG

REM 
REM Check for unknown arguments and continue
REM
if "%1" NEQ "" goto :USAGE

if defined DISABLE_USER_SESSION (
    echo.
    echo INFO: Deleting UserSession key under the AutoLogger hive...
    echo.
    reg.exe delete HKLM\SYSTEM\CurrentControlSet\Control\WMI\AutoLogger\UserSession /f
    del %windir%\system32\LogFiles\WMI\usertrace.log
    goto :EXIT
)

if not defined ADDITIONAL_USERPROVIDER_GUID (
    goto :USAGE
)

set ADDITIONAL_USERPROVIDER_GUID={%ADDITIONAL_USERPROVIDER_GUID%}
reg.exe delete HKLM\SYSTEM\CurrentControlSet\Control\WMI\AutoLogger\UserSession\%ADDITIONAL_USERPROVIDER_GUID% /f

echo.
echo INFO: Enabling user provider "%ADDITIONAL_USERPROVIDER_GUID%" in boot trace...
echo.
reg.exe add HKLM\SYSTEM\CurrentControlSet\Control\WMI\AutoLogger\UserSession /f
reg.exe add HKLM\SYSTEM\CurrentControlSet\Control\WMI\AutoLogger\UserSession /v GUID /t REG_SZ /d {b1fba409-283e-4698-91bb-48833dd9289f} /f
reg.exe add HKLM\SYSTEM\CurrentControlSet\Control\WMI\AutoLogger\UserSession /v BufferSize /t REG_DWORD /d 64 /f
reg.exe add HKLM\SYSTEM\CurrentControlSet\Control\WMI\AutoLogger\UserSession /v MinimumBuffers /t REG_DWORD /d 64 /f
reg.exe add HKLM\SYSTEM\CurrentControlSet\Control\WMI\AutoLogger\UserSession /v MaximumBuffers /t REG_DWORD /d 320 /f
reg.exe add HKLM\SYSTEM\CurrentControlSet\Control\WMI\AutoLogger\UserSession /v ClockType /t REG_DWORD /d 1 /f
reg.exe add HKLM\SYSTEM\CurrentControlSet\Control\WMI\AutoLogger\UserSession /v LogFileMode /t REG_DWORD /d 1 /f
reg.exe add HKLM\SYSTEM\CurrentControlSet\Control\WMI\AutoLogger\UserSession /v MaxFileSize /t REG_DWORD /d 0 /f
reg.exe add HKLM\SYSTEM\CurrentControlSet\Control\WMI\AutoLogger\UserSession /v FileName /t REG_SZ /d %windir%\system32\LogFiles\WMI\usertrace.log /f
reg.exe add HKLM\SYSTEM\CurrentControlSet\Control\WMI\AutoLogger\UserSession\%ADDITIONAL_USERPROVIDER_GUID% /f
reg.exe add HKLM\SYSTEM\CurrentControlSet\Control\WMI\AutoLogger\UserSession\%ADDITIONAL_USERPROVIDER_GUID% /v Enabled /t REG_DWORD /d 1 /f
reg.exe add HKLM\SYSTEM\CurrentControlSet\Control\WMI\AutoLogger\UserSession /v Start /t REG_DWORD /d 1 /f

if defined ADDITIONAL_USERPROVIDER_STACK (
   echo INFO: Also enabling stack walking for user provider %ADDITIONAL_USERPROVIDER_GUID%
   reg.exe add HKLM\SYSTEM\CurrentControlSet\Control\WMI\AutoLogger\UserSession\%ADDITIONAL_USERPROVIDER_GUID% /v EnableProperty /t REG_DWORD /d 0x4 /f
)

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
echo USAGE: edit_auto_logger.cmd [options] - edit user boot trace settings
echo. 
echo Options
echo        -enable - user provider
echo        -stack - enable stackwalk events for the user provider
echo.

goto :EOF
