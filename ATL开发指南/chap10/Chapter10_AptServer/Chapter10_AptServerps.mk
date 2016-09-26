
Chapter10_AptServerps.dll: dlldata.obj Chapter10_AptServer_p.obj Chapter10_AptServer_i.obj
	link /dll /out:Chapter10_AptServerps.dll /def:Chapter10_AptServerps.def /entry:DllMain dlldata.obj Chapter10_AptServer_p.obj Chapter10_AptServer_i.obj kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib 

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL $<

clean:
	@del Chapter10_AptServerps.dll
	@del Chapter10_AptServerps.lib
	@del Chapter10_AptServerps.exp
	@del dlldata.obj
	@del Chapter10_AptServer_p.obj
	@del Chapter10_AptServer_i.obj
