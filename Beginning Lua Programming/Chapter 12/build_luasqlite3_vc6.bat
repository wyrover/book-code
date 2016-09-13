cl /c /Zl /Zd /Yd /MD /DWIN32 libluasqlite3.c
link /dll /base:0x67A00000 /machine:ix86 /export:luaopen_sqlite3 libluasqlite3.obj lua5.1.lib msvcrt.lib sqlite3.lib
