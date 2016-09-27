
ATLSampleps.dll: dlldata.obj ATLSample_p.obj ATLSample_i.obj
	link /dll /out:ATLSampleps.dll /def:ATLSampleps.def /entry:DllMain dlldata.obj ATLSample_p.obj ATLSample_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del ATLSampleps.dll
	@del ATLSampleps.lib
	@del ATLSampleps.exp
	@del dlldata.obj
	@del ATLSample_p.obj
	@del ATLSample_i.obj
