
Chapter5_Aggregateps.dll: dlldata.obj Chapter5_Aggregate_p.obj Chapter5_Aggregate_i.obj
	link /dll /out:Chapter5_Aggregateps.dll /def:Chapter5_Aggregateps.def /entry:DllMain dlldata.obj Chapter5_Aggregate_p.obj Chapter5_Aggregate_i.obj kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib 

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL $<

clean:
	@del Chapter5_Aggregateps.dll
	@del Chapter5_Aggregateps.lib
	@del Chapter5_Aggregateps.exp
	@del dlldata.obj
	@del Chapter5_Aggregate_p.obj
	@del Chapter5_Aggregate_i.obj
