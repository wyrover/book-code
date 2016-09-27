
Chapter5_Simpleps.dll: dlldata.obj Chapter5_Simple_p.obj Chapter5_Simple_i.obj
	link /dll /out:Chapter5_Simpleps.dll /def:Chapter5_Simpleps.def /entry:DllMain dlldata.obj Chapter5_Simple_p.obj Chapter5_Simple_i.obj kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib 

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL $<

clean:
	@del Chapter5_Simpleps.dll
	@del Chapter5_Simpleps.lib
	@del Chapter5_Simpleps.exp
	@del dlldata.obj
	@del Chapter5_Simple_p.obj
	@del Chapter5_Simple_i.obj
