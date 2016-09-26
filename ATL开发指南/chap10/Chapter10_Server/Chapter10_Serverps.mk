
Chapter10_Serverps.dll: dlldata.obj Chapter10_Server_p.obj Chapter10_Server_i.obj
	link /dll /out:Chapter10_Serverps.dll /def:Chapter10_Serverps.def /entry:DllMain dlldata.obj Chapter10_Server_p.obj Chapter10_Server_i.obj kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib 

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL $<

clean:
	@del Chapter10_Serverps.dll
	@del Chapter10_Serverps.lib
	@del Chapter10_Serverps.exp
	@del dlldata.obj
	@del Chapter10_Server_p.obj
	@del Chapter10_Server_i.obj
