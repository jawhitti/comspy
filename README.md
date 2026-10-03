# COM Spy

COM Spy (originally the "control spy") is a 1999 utility for spying on COM objects. It shows which interfaces are held on an object, logs every method call with its HRESULT, and reports interfaces that containers leak. It works on any in-proc, apartment-threaded ActiveX control in any container.

It grew out of the DebugHook/CoDelegator tracing work by Chris Sells and Keith Brown.

![screenshot](archive/screenshot.gif)

## Layout

- `src/`: the Visual C++ 6 source, recovered from `comspy_src.zip` (January 2000) via the Wayback Machine
  - `ctlspysrv/`: ctlspysrv.dll, the core: the blind delegator and the hooking
  - `debugger/`: agent.exe, the taskbar helper process
  - `hookcfg/`: spycfg.exe, which hooks and unhooks classes
  - `spygui/`: comspy.exe, the MFC GUI
- `archive/`: the original web pages from staff.develop.com/jasonw/comspy, the v0.1 binary release (`comspy01.zip`), the source zip, and the screenshot

Per the original notes, this source is close to, but not exactly, what built the released binaries. Some of the MTA-spying work had already been started in it.

## Building (from the original notes)

Build each project, then `nmake ctlspysrv\ctlspysrvps.mk` and `debugger\debuggerps.mk` to build the proxy/stub DLLs, then register everything (`install.bat`).
