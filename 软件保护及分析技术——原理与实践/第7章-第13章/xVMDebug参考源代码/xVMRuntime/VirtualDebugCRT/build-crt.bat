@echo off
set tcc_inc="tcc/include/"
set tcc_win32_inc="tcc/win32/include/"
set tcc_win32_winapi="tcc/win32/include/winapi"
set tcc_win32_lib="tcc/win32/lib"
set tcc_bin_lib=..\_bin\inc\win32\lib\
set tcc_tools=..\_bin\inc\tools\
mkdir %tcc_tools%
mkdir %tcc_bin_lib%
tcc -c -I%tcc_inc% -I%tcc_win32_inc% -I%tcc_win32_winapi% -L%tcc_win32_lib% tcc/lib/libtcc1.c -o %tcc_bin_lib%/libtcc1.a
tcc -I%tcc_inc% -I%tcc_win32_inc% -I%tcc_win32_winapi% -L%tcc_win32_lib% -L"../_bin/inc/win32/lib/" tcc/tcc.c tcc/win32/lib/crt1.c tcc/win32/lib/chkstk.S -o tcc_new.exe
del /F /Q tcc.exe
ren tcc_new.exe tcc.exe
tcc -I%tcc_inc% -I%tcc_win32_inc% -I%tcc_win32_winapi% -L%tcc_win32_lib% -L"../_bin/inc/win32/lib/" tcc/win32/tools/tiny_impdef.c tcc/win32/lib/crt1.c tcc/win32/lib/chkstk.S -o ../_bin/inc/tools/tiny_impdef.exe
tcc -I%tcc_inc% -I%tcc_win32_inc% -I%tcc_win32_winapi% -L%tcc_win32_lib% -L"../_bin/inc/win32/lib/" tcc/win32/tools/tiny_libmaker.c tcc/win32/lib/crt1.c tcc/win32/lib/chkstk.S -o ../_bin/inc/tools/tiny_libmaker.exe
echo F|xcopy internal.h "../_bin/inc/internal/internal.h" /Y
xcopy "tcc\include\*" "..\_bin\inc\win32\include\" /E /Y /R /I /Q
xcopy "tcc\win32\include\*" "..\_bin\inc\win32\include\" /E /Y /R /I /Q
xcopy "tcc\win32\lib\*.def" "..\_bin\inc\win32\lib\" /E /Y /R /I /Q
tcc -c -I%tcc_inc% -I%tcc_win32_inc% -I%tcc_win32_winapi% -L%tcc_win32_lib% tcc/win32/lib/chkstk.S -o %tcc_bin_lib%/chkstk.a
tcc -c -I%tcc_inc% -I%tcc_win32_inc% -I%tcc_win32_winapi% -L%tcc_win32_lib% tcc/win32/lib/crt1.c -o %tcc_bin_lib%/crt1.a
tcc -c -I%tcc_inc% -I%tcc_win32_inc% -I%tcc_win32_winapi% -L%tcc_win32_lib% tcc/win32/lib/dllcrt1.c -o %tcc_bin_lib%/dllcrt1.a
tcc -c -I%tcc_inc% -I%tcc_win32_inc% -I%tcc_win32_winapi% -L%tcc_win32_lib% tcc/win32/lib/dllmain.c -o %tcc_bin_lib%/dllmain.a
tcc -c -I%tcc_inc% -I%tcc_win32_inc% -I%tcc_win32_winapi% -L%tcc_win32_lib% tcc/win32/lib/wincrt1.c -o %tcc_bin_lib%/wincrt1.a
%tcc_tools%\tiny_impdef.exe kernel32.dll -o %tcc_bin_lib%\kernel32.def
%tcc_tools%\tiny_impdef.exe user32.dll -o %tcc_bin_lib%\user32.def
%tcc_tools%\tiny_impdef.exe gdi32.dll -o %tcc_bin_lib%\gdi32.def
%tcc_tools%\tiny_impdef.exe shell32.dll -o %tcc_bin_lib%\shell32.def
%tcc_tools%\tiny_impdef.exe advapi32.dll -o %tcc_bin_lib%\advapi32.def
%tcc_tools%\tiny_impdef.exe msvcrt.dll -o %tcc_bin_lib%\msvcrt.def
@echo on