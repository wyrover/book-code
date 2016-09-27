
ASPComSampps.dll: dlldata.obj ASPComSamp_p.obj ASPComSamp_i.obj
	link /dll /out:ASPComSampps.dll /def:ASPComSampps.def /entry:DllMain dlldata.obj ASPComSamp_p.obj ASPComSamp_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del ASPComSampps.dll
	@del ASPComSampps.lib
	@del ASPComSampps.exp
	@del dlldata.obj
	@del ASPComSamp_p.obj
	@del ASPComSamp_i.obj
