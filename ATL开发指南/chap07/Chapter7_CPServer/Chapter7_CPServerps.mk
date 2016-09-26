
Chapter7_CPServerps.dll: dlldata.obj Chapter7_CPServer_p.obj Chapter7_CPServer_i.obj
	link /dll /out:Chapter7_CPServerps.dll /def:Chapter7_CPServerps.def /entry:DllMain dlldata.obj Chapter7_CPServer_p.obj Chapter7_CPServer_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del Chapter7_CPServerps.dll
	@del Chapter7_CPServerps.lib
	@del Chapter7_CPServerps.exp
	@del dlldata.obj
	@del Chapter7_CPServer_p.obj
	@del Chapter7_CPServer_i.obj
