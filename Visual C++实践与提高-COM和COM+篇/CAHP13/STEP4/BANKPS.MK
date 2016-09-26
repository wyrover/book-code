
Bankps.dll: dlldata.obj Bank_p.obj Bank_i.obj
	link /dll /out:Bankps.dll /def:Bankps.def /entry:DllMain dlldata.obj Bank_p.obj Bank_i.obj \
		mtxih.lib mtx.lib mtxguid.lib \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \
		ole32.lib advapi32.lib 

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		/MD \
		$<

clean:
	@del Bankps.dll
	@del Bankps.lib
	@del Bankps.exp
	@del dlldata.obj
	@del Bank_p.obj
	@del Bank_i.obj
