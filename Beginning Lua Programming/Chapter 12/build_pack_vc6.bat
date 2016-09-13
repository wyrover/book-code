cl /c /W1 /Zl /Zd /Yd /MD /DWIN32 lpack.c
link /dll /out:pack.dll /base:0x67400000 /machine:ix86 /export:luaopen_pack lpack.obj lua5.1.lib msvcrt.lib
