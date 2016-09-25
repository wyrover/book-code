@ECHO OFF

ECHO.
ECHO Creating patched copies of winload, ntkrnlmp/ntoskrnl...
ECHO.
patch.exe

ECHO.
ECHO Creating BCD Entry...
ECHO.
set ENTRY_GUID={46595952-454E-4F50-4747-554944FFFFFF}
bcdedit -create %ENTRY_GUID% -d "DriverSigning&PatchGuard Disabled" -application OSLOADER
bcdedit -set %ENTRY_GUID% device partition=%SYSTEMDRIVE%
bcdedit -set %ENTRY_GUID% osdevice partition=%SYSTEMDRIVE%
bcdedit -set %ENTRY_GUID% systemroot \Windows
bcdedit -set %ENTRY_GUID% path \Windows\system32\freeload.exe
bcdedit -set %ENTRY_GUID% kernel goodkrnl.exe
bcdedit -set %ENTRY_GUID% recoveryenabled 0
bcdedit -set %ENTRY_GUID% nx OptOut
bcdedit -set %ENTRY_GUID% nointegritychecks 1
bcdedit -set %ENTRY_GUID% testsigning 1
bcdedit -displayorder %ENTRY_GUID% -addlast
bcdedit -timeout 5
bcdedit -default %ENTRY_GUID%

ECHO.
ECHO Setting PEAUTH service to manual... (avoid BSOD at login screen)
ECHO.
sc config peauth start= demand

ECHO.
ECHO Complete!
ECHO.
PAUSE
