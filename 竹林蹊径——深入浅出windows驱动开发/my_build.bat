@echo on
@echo %1 is chk or fre %3 is WXP or W2K or WNET.

if "%4"=="/a" call my_clean %1 %2 %3

pushd.
call %BASEDIR%\bin\setenv.bat %BASEDIR% %1 %2 %3
popd

@echo on
build