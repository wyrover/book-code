
ATLSampleExeps.dll: dlldata.obj ATLSampleExe_p.obj ATLSampleExe_i.obj
	link /dll /out:ATLSampleExeps.dll /def:ATLSampleExeps.def /entry:DllMain dlldata.obj ATLSampleExe_p.obj ATLSampleExe_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del ATLSampleExeps.dll
	@del ATLSampleExeps.lib
	@del ATLSampleExeps.exp
	@del dlldata.obj
	@del ATLSampleExe_p.obj
	@del ATLSampleExe_i.obj
