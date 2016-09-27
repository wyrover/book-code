
Chapter4_Serverps.dll: dlldata.obj Chapter4_Server_p.obj Chapter4_Server_i.obj
	link /dll /out:Chapter4_Serverps.dll /def:Chapter4_Serverps.def /entry:DllMain dlldata.obj Chapter4_Server_p.obj Chapter4_Server_i.obj kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib 

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL $<

clean:
	@del Chapter4_Serverps.dll
	@del Chapter4_Serverps.lib
	@del Chapter4_Serverps.exp
	@del dlldata.obj
	@del Chapter4_Server_p.obj
	@del Chapter4_Server_i.obj
