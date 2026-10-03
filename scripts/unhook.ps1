<#
.SYNOPSIS
    Restores classes hooked by hook.ps1 (or by spycfg.exe) to their original registration.

.DESCRIPTION
    If hook.ps1 left a backup for the class, it is re-imported, which restores the original value
    and its type exactly. Otherwise (a class hooked with spycfg) the path is taken from
    HookSavedPath, written back as REG_EXPAND_SZ if it contains %variables%.

.EXAMPLE
    .\unhook.ps1 MSCAL.Calendar.7
    .\unhook.ps1 -All
#>
param(
    [Parameter(ParameterSetName = 'One', Mandatory, Position = 0)][string]$Class,
    [Parameter(ParameterSetName = 'All', Mandatory)][switch]$All,
    [string]$Dest = 'C:\comspy'
)
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'common.ps1')
Assert-Admin

function Restore-Class([string]$clsid) {
    $keyPath    = Get-InprocKeyPath $clsid
    $backupFile = Join-Path $Dest "backup\hook-$clsid.reg"
    $key = Open-Hklm $keyPath $true
    if (-not $key) { Write-Host "  skip    $clsid (no InprocServer32 key)"; return }
    try {
        $saved = $key.GetValue('HookSavedPath', $null, 'DoNotExpandEnvironmentNames')
        if (Test-Path $backupFile) {
            & reg.exe import $backupFile 2>$null
            if ($LASTEXITCODE -ne 0) { throw "reg import of $backupFile failed." }
            $how = 'restored from backup'
        } elseif ($null -ne $saved) {
            $kind = if ($saved -match '%') { 'ExpandString' } else { 'String' }
            $key.SetValue('', $saved, $kind)
            $how = "restored from HookSavedPath as $kind"
        } else {
            Write-Host "  skip    $clsid (not hooked)"; return
        }
        # reg import merges, so the hook's own value has to be removed explicitly.
        if ($null -ne $key.GetValue('HookSavedPath')) { $key.DeleteValue('HookSavedPath') }
        Write-Host "  OK      $clsid ($how): $($key.GetValue('', $null, 'DoNotExpandEnvironmentNames'))"
        if (Test-Path $backupFile) { Rename-Item $backupFile "$backupFile.restored" -Force }
    } finally { $key.Close() }
}

if ($All) {
    Write-Host 'Looking for hooked 32-bit classes'
    $clsidRoot = Open-Hklm "$Classes32\CLSID"
    $hooked = foreach ($name in $clsidRoot.GetSubKeyNames()) {
        $k = $clsidRoot.OpenSubKey("$name\InprocServer32")
        if ($k) { if ($null -ne $k.GetValue('HookSavedPath')) { $name }; $k.Close() }
    }
    $clsidRoot.Close()
    if (-not $hooked) { Write-Host '  none found'; return }
    $hooked | ForEach-Object { Restore-Class $_ }
} else {
    Restore-Class (Resolve-Clsid $Class)
}
