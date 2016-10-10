mode con cp select=936
cd /d "%~dp0"
if exist "%~dp0src_tools\AStyle.exe" (
set astyle=%~dp0src_tools\AStyle.exe --style=linux --s4 --p --H --U --f --v --w --c --xe --xL --xW
)

if exist "%~dp0..\src_tools\AStyle.exe" (
set astyle=%~dp0..\src_tools\AStyle.exe --style=linux --s4 --p --H --U --f --v --w --c --xe --xL --xW
)

set dir_path="%~dp0"
echo "format source code......"
for /R %dir_path% %%a in (*.cpp;*.c;*.cc;*.h;*.hpp) do %astyle% "%%a" 1>nul 2>nul
echo "delete backup source code......"
for /R %dir_path% %%a in (*.orig) do del "%%a"
echo "format source code end."


for %%i in (.) do (
pushd %%i
::echo %%i
del /F /S /Q *.i *.exp *.aps *.idb *.ncp *.obj *.pch *.sbr *.tmp *.bsc *.ilk *.res *.ncb *.opt *.suo *.dep *.user *.plg *.clw *.orig *.DS_Store
del *.suo /s /Q /ah
popd
)

pause


