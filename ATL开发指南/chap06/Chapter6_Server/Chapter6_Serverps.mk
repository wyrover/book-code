
Chapter6_Serverps.dll: dlldata.obj Chapter6_Server_p.obj Chapter6_Server_i.obj
	link /dll /out:Chapter6_Serverps.dll /def:Chapter6_Serverps.def /entry:DllMain dlldata.obj Chapter6_Server_p.obj Chapter6_Server_i.obj kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib 

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL $<

clean:
	@del Chapter6_Serverps.dll
	@del Chapter6_Serverps.lib
	@del Chapter6_Serverps.exp
	@del dlldata.obj
	@del Chapter6_Server_p.obj
	@del Chapter6_Server_i.obj
