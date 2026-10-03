# Shared helpers for the COM Spy install/hook scripts. Dot-sourced, not run directly.

# COM Spy is 32-bit, so everything it touches lives in the WOW64 view of HKCR.
$Classes32   = 'SOFTWARE\Classes\WOW6432Node'
$SpyClsid    = '{562BFEB0-3310-11D3-8BF2-00105A6DC077}'   # ctlspysrv.dll (Spy.rgs)
$AgentClsids = '{58C849A0-3314-11D3-8BF2-00105A6DC077}',  # Debugger.Debugger
               '{664E10F0-3F2B-11D3-8BFA-00105A6DC077}'   # Debugger.ClientAccessor
$Regsvr32    = "$env:SystemRoot\SysWOW64\regsvr32.exe"
$DefaultDest = 'C:\comspy'

function Assert-Admin {
    $id = [Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()
    if (-not $id.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
        throw 'Run this from an elevated (Run as administrator) PowerShell.'
    }
}

function Open-Hklm([string]$Path, [bool]$Writable = $false) {
    [Microsoft.Win32.Registry]::LocalMachine.OpenSubKey($Path, $Writable)
}

# Runs a GUI-subsystem program (regsvr32, agent.exe) and waits for its exit code.
function Invoke-Checked([string]$Exe, [string[]]$Arguments, [string]$What) {
    $p = Start-Process -FilePath $Exe -ArgumentList $Arguments -Wait -PassThru
    if ($p.ExitCode -eq 0) {
        Write-Host "  OK      $What"
    } else {
        # regsvr32: 3 = LoadLibrary failed, 4 = entry point not found, 5 = the call failed
        Write-Host "  FAILED  $What (exit code $($p.ExitCode))" -ForegroundColor Red
    }
    $p.ExitCode
}

# Accepts a ProgID (MSCAL.Calendar.7) or a CLSID ({...}) and returns the CLSID.
function Resolve-Clsid([string]$Class) {
    if ($Class -match '^\{[0-9A-Fa-f-]{36}\}$') { return $Class.ToUpper() }
    foreach ($root in 'SOFTWARE\Classes', $Classes32) {
        $k = Open-Hklm "$root\$Class\CLSID"
        if ($k) { $v = $k.GetValue(''); $k.Close(); if ($v) { return $v.ToUpper() } }
    }
    throw "Can't find a CLSID for ProgID '$Class'."
}

function Get-InprocKeyPath([string]$Clsid) { "$Classes32\CLSID\$Clsid\InprocServer32" }

function Get-SpyDllPath {
    $k = Open-Hklm (Get-InprocKeyPath $SpyClsid)
    if (-not $k) { throw 'COM Spy is not registered. Run install.ps1 first.' }
    try { $k.GetValue('') } finally { $k.Close() }
}
