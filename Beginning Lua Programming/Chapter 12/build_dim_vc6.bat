cl /MD /O2 /W3 /c /DLUA_BUILD_AS_DLL min.c
link /OUT:min.exe min.obj lua5.1.lib
