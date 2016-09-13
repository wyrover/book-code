cl /c /Zl /Zd /Yd /MD /W4 ud_example.c
link /dll /out:ud_example.dll /base:0x68200000 /machine:ix86 /export:luaopen_ud_example ud_example.obj msvcrt.lib lua5.1.lib
