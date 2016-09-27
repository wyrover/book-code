
DCOMServps.dll: dlldata.obj DCOMServ_p.obj DCOMServ_i.obj
	link /dll /out:DCOMServps.dll /def:DCOMServps.def /entry:DllMain dlldata.obj DCOMServ_p.obj DCOMServ_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del DCOMServps.dll
	@del DCOMServps.lib
	@del DCOMServps.exp
	@del dlldata.obj
	@del DCOMServ_p.obj
	@del DCOMServ_i.obj
