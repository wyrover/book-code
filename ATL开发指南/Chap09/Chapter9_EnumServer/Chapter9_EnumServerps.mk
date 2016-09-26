
Chapter9_EnumServerps.dll: dlldata.obj Chapter9_EnumServer_p.obj Chapter9_EnumServer_i.obj
	link /dll /out:Chapter9_EnumServerps.dll /def:Chapter9_EnumServerps.def /entry:DllMain dlldata.obj Chapter9_EnumServer_p.obj Chapter9_EnumServer_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del Chapter9_EnumServerps.dll
	@del Chapter9_EnumServerps.lib
	@del Chapter9_EnumServerps.exp
	@del dlldata.obj
	@del Chapter9_EnumServer_p.obj
	@del Chapter9_EnumServer_i.obj
