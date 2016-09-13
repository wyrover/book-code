SET TCCDIR=c:\program files\tcc-0.9.23
SET TCCCMD="%TCCDIR%\tcc\tcc" -D_WIN32 -I"%TCCDIR%\include" -I"%TCCDIR%\include\winapi" -L"%TCCDIR%\lib" -lkernel32
ren luac.c luac.c0
ren print.c print.c0
%TCCCMD% -o lua.exe *.c
ren lua.c lua.c0
ren luac.c0 luac.c
ren print.c0 print.c
%TCCCMD% -o luac.exe *.c
ren lua.c0 lua.c
SET TCCDIR=
SET TCCCMD=
SET TCCIMP=
