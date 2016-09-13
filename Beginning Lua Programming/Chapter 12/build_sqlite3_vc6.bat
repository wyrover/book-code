cl /c /nologo /I. /Zl /Zd /Yd /MD /W0 *.c
ren shell.obj shell.o
link /DLL /OUT:sqlite3.dll /BASE:0x66000000 /DEF:sqlite3.def *.obj msvcrt.lib
link shell.o /OUT:sqlite3.exe sqlite3.lib msvcrt.lib
