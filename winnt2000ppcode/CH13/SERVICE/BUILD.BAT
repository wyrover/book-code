rem Uncomment one of the following two lines:

set BUILD=bcc
rem set BUILD=msc

if NOT "%BUILD%"=="msc" goto NOTMSC
cl instsrv.c advapi32.lib
cl wdjsrvc.c srvctmpl.c user32.lib advapi32.lib
goto END
:NOTMSC

if NOT "%BUILD%"=="bcc" goto NOTBCC
bcc32 instsrv.c
bcc32 wdjsrvc.c srvctmpl.c
goto END
:NOTBCC

:END
