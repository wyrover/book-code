cl /c /Zl /Zd /Yd /MD /W4 /DWIN32 csvparse.c
link /dll /out:csvparse.dll /base:0x67C00000 /machine:ix86 /export:luaopen_csvparse csvparse.obj msvcrt.lib lua5.1.lib
