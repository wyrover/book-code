
Chapter10_BothServerps.dll: dlldata.obj Chapter10_BothServer_p.obj Chapter10_BothServer_i.obj
	link /dll /out:Chapter10_BothServerps.dll /def:Chapter10_BothServerps.def /entry:DllMain dlldata.obj Chapter10_BothServer_p.obj Chapter10_BothServer_i.obj kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib 

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL $<

clean:
	@del Chapter10_BothServerps.dll
	@del Chapter10_BothServerps.lib
	@del Chapter10_BothServerps.exp
	@del dlldata.obj
	@del Chapter10_BothServer_p.obj
	@del Chapter10_BothServer_i.obj
