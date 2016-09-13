cl /c /W1 /Zl /Zd /Yd /MD /DWIN32 luacurl.c
link /dll /base:0x68000000 /machine:ix86 /export:luaopen_luacurl luacurl.obj lua5.1.lib msvcrt.lib libcurl_imp.lib
