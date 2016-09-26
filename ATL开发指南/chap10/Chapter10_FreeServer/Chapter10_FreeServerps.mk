
Chapter10_FreeServerps.dll: dlldata.obj Chapter10_FreeServer_p.obj Chapter10_FreeServer_i.obj
	link /dll /out:Chapter10_FreeServerps.dll /def:Chapter10_FreeServerps.def /entry:DllMain dlldata.obj Chapter10_FreeServer_p.obj Chapter10_FreeServer_i.obj kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib 

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL $<

clean:
	@del Chapter10_FreeServerps.dll
	@del Chapter10_FreeServerps.lib
	@del Chapter10_FreeServerps.exp
	@del dlldata.obj
	@del Chapter10_FreeServer_p.obj
	@del Chapter10_FreeServer_i.obj
