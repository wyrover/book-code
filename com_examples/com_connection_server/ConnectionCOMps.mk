
ConnectionCOMps.dll: dlldata.obj ConnectionCOM_p.obj ConnectionCOM_i.obj
	link /dll /out:ConnectionCOMps.dll /def:ConnectionCOMps.def /entry:DllMain dlldata.obj ConnectionCOM_p.obj ConnectionCOM_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del ConnectionCOMps.dll
	@del ConnectionCOMps.lib
	@del ConnectionCOMps.exp
	@del dlldata.obj
	@del ConnectionCOM_p.obj
	@del ConnectionCOM_i.obj
