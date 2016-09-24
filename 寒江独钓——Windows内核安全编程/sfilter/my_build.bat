@echo on
@echo %1 is chk or fre %2 is WXP or W2K or WNET.

if "%3"=="/a" call my_clean %1 %2

pushd.
call %BASEDIR%\bin\setenv.bat %BASEDIR% %1 %2
popd

set INCLUDE=%INCLUDE%;%BASEDIR%\inc\ddk\%2;%BASEDIR%\inc\ddk\wdm\%2;..\inc

@echo on
build

if not exist ..\inc mkdir ..\inc
if not exist ..\inc\sfilter mkdir ..\inc\sfilter
if not exist ..\lib mkdir ..\lib
copy *.h ..\inc\sfilter\
copy .\obj%1_%2_x86\i386\sfilter.lib ..\lib\sfilter_%1_%2.lib