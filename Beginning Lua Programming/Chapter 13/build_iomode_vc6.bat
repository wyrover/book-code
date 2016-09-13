cl /c /Zl /Zd /Yd /MD /W4 /DWIN32 iomode.c
link /dll /out:iomode.dll /base:0x67900000 /machine:ix86 /export:luaopen_iomode iomode.obj msvcrt.lib lua5.1.lib
