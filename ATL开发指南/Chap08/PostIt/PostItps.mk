
PostItps.dll: dlldata.obj PostIt_p.obj PostIt_i.obj
	link /dll /out:PostItps.dll /def:PostItps.def /entry:DllMain dlldata.obj PostIt_p.obj PostIt_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del PostItps.dll
	@del PostItps.lib
	@del PostItps.exp
	@del dlldata.obj
	@del PostIt_p.obj
	@del PostIt_i.obj
