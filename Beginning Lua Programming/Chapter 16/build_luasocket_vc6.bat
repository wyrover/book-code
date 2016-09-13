cl /c /nologo /Zl /Zd /Yd /MD /W3 /DWIN32 /DWIN32_LEAN_AND_MEAN *.c
ren mime.obj mime.o
link /DLL /out:socket.dll /base:0x67800000 /export:luaopen_socket_core *.obj msvcrt.lib lua5.1.lib wsock32.lib
link /DLL /out:mime.dll /base:0x67700000 /export:luaopen_mime_core mime.o msvcrt.lib lua5.1.lib
