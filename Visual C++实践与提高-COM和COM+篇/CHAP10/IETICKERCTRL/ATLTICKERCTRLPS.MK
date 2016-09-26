
AtlTickerCtrlps.dll: dlldata.obj AtlTickerCtrl_p.obj AtlTickerCtrl_i.obj
	link /dll /out:AtlTickerCtrlps.dll /def:AtlTickerCtrlps.def /entry:DllMain dlldata.obj AtlTickerCtrl_p.obj AtlTickerCtrl_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del AtlTickerCtrlps.dll
	@del AtlTickerCtrlps.lib
	@del AtlTickerCtrlps.exp
	@del dlldata.obj
	@del AtlTickerCtrl_p.obj
	@del AtlTickerCtrl_i.obj
