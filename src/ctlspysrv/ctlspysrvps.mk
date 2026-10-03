
ctlspysrvps.dll: dlldata.obj ctlspysrv_p.obj ctlspysrv_i.obj
	link /dll /out:ctlspysrvps.dll /def:ctlspysrvps.def /entry:DllMain dlldata.obj ctlspysrv_p.obj ctlspysrv_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del ctlspysrvps.dll
	@del ctlspysrvps.lib
	@del ctlspysrvps.exp
	@del dlldata.obj
	@del ctlspysrv_p.obj
	@del ctlspysrv_i.obj
