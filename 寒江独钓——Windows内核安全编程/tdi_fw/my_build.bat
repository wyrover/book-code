@echo on
@echo %1 is chk or fre %2 is WXP or W2K or WNET.

if "%3"=="/a" call my_clean %1 %2

pushd.
call %BASEDIR%\bin\setenv.bat %BASEDIR% %1 %2
popd

set INCLUDE=%INCLUDE%;%BASEDIR%\inc\ddk\;%BASEDIR%\inc\%2\

@echo on
build

if not exist ..\inc mkdir ..\inc
if not exist ..\inc\tdi_fw mkdir ..\inc\tdi_fw
if not exist ..\lib mkdir ..\lib
copy *.h ..\inc\tdi_fw\
copy .\obj%1_%2_x86\i386\tdi_fw.lib ..\lib\tdi_fw_%1_%2.lib