
ATL_OLEDB_Provps.dll: dlldata.obj ATL_OLEDB_Prov_p.obj ATL_OLEDB_Prov_i.obj
	link /dll /out:ATL_OLEDB_Provps.dll /def:ATL_OLEDB_Provps.def /entry:DllMain dlldata.obj ATL_OLEDB_Prov_p.obj ATL_OLEDB_Prov_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del ATL_OLEDB_Provps.dll
	@del ATL_OLEDB_Provps.lib
	@del ATL_OLEDB_Provps.exp
	@del dlldata.obj
	@del ATL_OLEDB_Prov_p.obj
	@del ATL_OLEDB_Prov_i.obj
