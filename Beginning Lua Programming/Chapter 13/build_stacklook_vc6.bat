cl /c /Zl /Zd /Yd /MD /W4 /DWIN32 stacklook.c
link /dll /out:stacklook.dll /base:0x68100000 /machine:ix86 /export:luaopen_stacklook stacklook.obj msvcrt.lib lua5.1.lib
