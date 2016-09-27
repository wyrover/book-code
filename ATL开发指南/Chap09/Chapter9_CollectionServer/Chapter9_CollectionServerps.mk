
Chapter9_CollectionServerps.dll: dlldata.obj Chapter9_CollectionServer_p.obj Chapter9_CollectionServer_i.obj
	link /dll /out:Chapter9_CollectionServerps.dll /def:Chapter9_CollectionServerps.def /entry:DllMain dlldata.obj Chapter9_CollectionServer_p.obj Chapter9_CollectionServer_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del Chapter9_CollectionServerps.dll
	@del Chapter9_CollectionServerps.lib
	@del Chapter9_CollectionServerps.exp
	@del dlldata.obj
	@del Chapter9_CollectionServer_p.obj
	@del Chapter9_CollectionServer_i.obj
