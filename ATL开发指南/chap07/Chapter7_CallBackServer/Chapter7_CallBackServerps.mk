
Chapter7_CallBackServerps.dll: dlldata.obj Chapter7_CallBackServer_p.obj Chapter7_CallBackServer_i.obj
	link /dll /out:Chapter7_CallBackServerps.dll /def:Chapter7_CallBackServerps.def /entry:DllMain dlldata.obj Chapter7_CallBackServer_p.obj Chapter7_CallBackServer_i.obj kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib 

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL $<

clean:
	@del Chapter7_CallBackServerps.dll
	@del Chapter7_CallBackServerps.lib
	@del Chapter7_CallBackServerps.exp
	@del dlldata.obj
	@del Chapter7_CallBackServer_p.obj
	@del Chapter7_CallBackServer_i.obj
