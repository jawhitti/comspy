# Trying the 1999 binaries on modern Windows

These scripts install the original `comspy01.zip` release on 64-bit Windows 10/11 and give it something safe to spy on. The 1999 binaries work unmodified (verified on Windows 10 22H2, October 2026). Run the scripts from an **elevated** PowerShell (they all check), but see *Elevation* below for running the spy itself. COM Spy is 32-bit, so it only sees controls loaded into 32-bit processes; everything here works in the WOW64 (32-bit) view of the registry.

```powershell
cd F:\Jason\develop\comspy\scripts
.\install.ps1                      # unzip to C:\comspy and register
.\test-target.ps1                  # register ComSpy.TestObject (test\ComSpyTest.wsc)
.\hook.ps1 ComSpy.TestObject       # point the class at the spy
# then, from a normal (non-admin) PowerShell:
C:\comspy\comspy.exe               # start the GUI (agent.exe should appear in the tray)
C:\Windows\SysWOW64\mshta.exe (Resolve-Path ..\test\test.hta)   # 32-bit host: Create, Ping, Fail, Release
                                   # (mshta needs a full path; with a relative one it silently exits)
```

To clean up:

```powershell
.\uninstall.ps1                    # unhook everything and unregister
.\test-target.ps1 -Remove
```

## Why not just install.bat and spycfg?

- `install.bat` runs everything with `/s`, so failures are invisible. `install.ps1` reports each exit code and checks that the registrations exist.
- About half of today's 32-bit COM registrations are `REG_EXPAND_SZ` (`%SystemRoot%\...`). spycfg copies those into `HookSavedPath` unexpanded, so the spy can't load the real DLL. Unhooking then writes them back as `REG_SZ`, which breaks the class. `hook.ps1` stores the expanded path and keeps a `.reg` backup of the key, which `unhook.ps1` re-imports exactly.
- Windows' own controls are owned by TrustedInstaller and are read-only even for Administrators, so they can't be hooked. `ComSpy.TestObject` is a Windows Script Component we register and own.

## Elevation

Only `install.ps1`, `hook.ps1`, `unhook.ps1`, `uninstall.ps1` and `test-target.ps1` need admin. Run **`comspy.exe` and the host process at the same level, normally both non-elevated**. If they differ, the hooked object starts its own `agent.exe` at its own level, and spygui shows nothing.

## Hosts

COM Spy 0.1 needs a host with a message loop. Good 32-bit hosts are `SysWOW64\mshta.exe` (used by `test.hta`) and 32-bit Office's VBA. `cscript` has no message loop.
