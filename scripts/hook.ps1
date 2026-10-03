<#
.SYNOPSIS
    Hooks one 32-bit in-proc COM class so COM Spy sees it. Use this instead of spycfg.exe.

.DESCRIPTION
    Does the same InprocServer32 swap as spycfg, with two fixes for modern Windows:

    - Many registrations today are REG_EXPAND_SZ (e.g. %SystemRoot%\SysWOW64\foo.ocx). spycfg
      copies them into HookSavedPath unexpanded, so ctlspysrv can't load the real DLL, and
      unhooking writes them back as REG_SZ, which breaks the class. This script stores the
      expanded path and keeps a .reg backup so unhook.ps1 can restore the exact original.
    - It refuses classes that aren't apartment-threaded, which COM Spy 0.1 can't handle,
      unless you pass -Force.

.EXAMPLE
    .\hook.ps1 MSCAL.Calendar.7
    .\hook.ps1 '{8E27C92B-1264-101C-8A2F-040224009C02}'
#>
param(
    [Parameter(Mandatory)][string]$Class,
    [string]$Dest = 'C:\comspy',
    [switch]$Force
)
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'common.ps1')
Assert-Admin

$clsid   = Resolve-Clsid $Class
$keyPath = Get-InprocKeyPath $clsid
$spyDll  = Get-SpyDllPath

$key = Open-Hklm $keyPath $true
if (-not $key) { throw "$Class ($clsid) has no 32-bit InprocServer32 registration, so COM Spy can't hook it." }
try {
    if ($null -ne $key.GetValue('HookSavedPath')) { Write-Host "$Class is already hooked."; return }

    $raw   = $key.GetValue('', $null, 'DoNotExpandEnvironmentNames')
    $kind  = $key.GetValueKind('')
    $real  = [Environment]::ExpandEnvironmentVariables($raw)
    $model = $key.GetValue('ThreadingModel')

    if (-not (Test-Path $real)) { throw "The registered DLL doesn't exist: $real" }
    if ($model -ne 'Apartment' -and -not $Force) {
        throw "ThreadingModel is '$model'. COM Spy 0.1 only supports 'Apartment'; pass -Force to try anyway."
    }

    $backupDir = Join-Path $Dest 'backup'
    New-Item -ItemType Directory -Force -Path $backupDir | Out-Null
    $backupFile = Join-Path $backupDir "hook-$clsid.reg"
    & reg.exe export "HKLM\$keyPath" $backupFile /y | Out-Null
    if ($LASTEXITCODE -ne 0) { throw 'Could not back up the InprocServer32 key; nothing changed.' }

    $key.SetValue('HookSavedPath', $real, 'String')
    $key.SetValue('', $spyDll, 'String')

    Write-Host "Hooked $Class $clsid"
    Write-Host "  real DLL : $real ($kind, ThreadingModel=$model)"
    Write-Host "  now loads: $spyDll"
    Write-Host "  backup   : $backupFile"
} finally { $key.Close() }
