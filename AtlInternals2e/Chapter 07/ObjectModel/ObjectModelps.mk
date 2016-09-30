
ObjectModelps.dll: dlldata.obj ObjectModel_p.obj ObjectModel_i.obj
	link /dll /out:ObjectModelps.dll /def:ObjectModelps.def /entry:DllMain dlldata.obj ObjectModel_p.obj ObjectModel_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del ObjectModelps.dll
	@del ObjectModelps.lib
	@del ObjectModelps.exp
	@del dlldata.obj
	@del ObjectModel_p.obj
	@del ObjectModel_i.obj
