
Chapter10_GITServerps.dll: dlldata.obj Chapter10_GITServer_p.obj Chapter10_GITServer_i.obj
	link /dll /out:Chapter10_GITServerps.dll /def:Chapter10_GITServerps.def /entry:DllMain dlldata.obj Chapter10_GITServer_p.obj Chapter10_GITServer_i.obj kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib 

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL $<

clean:
	@del Chapter10_GITServerps.dll
	@del Chapter10_GITServerps.lib
	@del Chapter10_GITServerps.exp
	@del dlldata.obj
	@del Chapter10_GITServer_p.obj
	@del Chapter10_GITServer_i.obj
