
Chapter3_Serverps.dll: dlldata.obj Chapter3_Server_p.obj Chapter3_Server_i.obj
	link /dll /out:Chapter3_Serverps.dll /def:Chapter3_Serverps.def /entry:DllMain dlldata.obj Chapter3_Server_p.obj Chapter3_Server_i.obj kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib 

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL $<

clean:
	@del Chapter3_Serverps.dll
	@del Chapter3_Serverps.lib
	@del Chapter3_Serverps.exp
	@del dlldata.obj
	@del Chapter3_Server_p.obj
	@del Chapter3_Server_i.obj
