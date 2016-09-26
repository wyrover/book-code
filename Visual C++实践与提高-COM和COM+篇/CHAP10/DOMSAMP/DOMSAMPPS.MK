
DOMSampps.dll: dlldata.obj DOMSamp_p.obj DOMSamp_i.obj
	link /dll /out:DOMSampps.dll /def:DOMSampps.def /entry:DllMain dlldata.obj DOMSamp_p.obj DOMSamp_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del DOMSampps.dll
	@del DOMSampps.lib
	@del DOMSampps.exp
	@del dlldata.obj
	@del DOMSamp_p.obj
	@del DOMSamp_i.obj
