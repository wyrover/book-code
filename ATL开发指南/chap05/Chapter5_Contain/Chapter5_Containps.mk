
Chapter5_Containps.dll: dlldata.obj Chapter5_Contain_p.obj Chapter5_Contain_i.obj
	link /dll /out:Chapter5_Containps.dll /def:Chapter5_Containps.def /entry:DllMain dlldata.obj Chapter5_Contain_p.obj Chapter5_Contain_i.obj kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib 

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL $<

clean:
	@del Chapter5_Containps.dll
	@del Chapter5_Containps.lib
	@del Chapter5_Containps.exp
	@del dlldata.obj
	@del Chapter5_Contain_p.obj
	@del Chapter5_Contain_i.obj
