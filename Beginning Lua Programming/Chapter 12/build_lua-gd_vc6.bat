cl /c /W1 /Zl /Zd /Yd /MD /DWIN32 /DGD_PNG /DGD_GIF /DGD_FREETYPE /DGD_JPEG /DGD_XPM /DGD_FONTCONFIG luagd.c
link /dll /out:gd.dll /base:0x67500000 /machine:ix86 /export:luaopen_gd luagd.obj lua5.1.lib msvcrt.lib bgd.lib
