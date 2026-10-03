
debuggerps.dll: dlldata.obj debugger_p.obj debugger_i.obj
	link /dll /out:debuggerps.dll /def:debuggerps.def /entry:DllMain dlldata.obj debugger_p.obj debugger_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del debuggerps.dll
	@del debuggerps.lib
	@del debuggerps.exp
	@del dlldata.obj
	@del debugger_p.obj
	@del debugger_i.obj
