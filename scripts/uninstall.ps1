<#
.SYNOPSIS
    Unhooks everything, unregisters COM Spy, and optionally deletes its folder.

.EXAMPLE
    .\uninstall.ps1
    .\uninstall.ps1 -RemoveFiles
#>
param(
    [string]$Dest = 'C:\comspy',
    [switch]$RemoveFiles
)
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'common.ps1')
Assert-Admin

Write-Host 'Stopping COM Spy processes'
Get-Process comspy, agent, spycfg -ErrorAction SilentlyContinue |
    Where-Object { $_.Path -like "$Dest\*" } |
    ForEach-Object { Stop-Process -Id $_.Id -Force; Write-Host "  stopped $($_.Name) ($($_.Id))" }

# Unhook first: once ctlspysrv.dll is unregistered, hooked classes would point at nothing.
& (Join-Path $PSScriptRoot 'unhook.ps1') -All -Dest $Dest

Write-Host 'Unregistering COM Spy'
$agent = Join-Path $Dest 'agent.exe'
if (Test-Path $agent) { Invoke-Checked $agent @('/UnregServer') 'agent /UnregServer' | Out-Null }
foreach ($dll in 'debuggerps.dll', 'ctlspysrvps.dll', 'ctlspysrv.dll') {
    $path = Join-Path $Dest $dll
    if (Test-Path $path) { Invoke-Checked $Regsvr32 @('/u', '/s', "`"$path`"") "regsvr32 /u $dll" | Out-Null }
}

Write-Host 'Checking registrations are gone'
$left = 0
foreach ($clsid in @($SpyClsid) + $AgentClsids) {
    $k = Open-Hklm "$Classes32\CLSID\$clsid"
    if ($k) { Write-Host "  STILL THERE $clsid" -ForegroundColor Yellow; $k.Close(); $left++ }
    else    { Write-Host "  gone    $clsid" }
}
if ($left) {
    Write-Host 'Some keys remain; delete them by hand under HKLM\SOFTWARE\Classes\WOW6432Node\CLSID.' -ForegroundColor Yellow
}

if ($RemoveFiles) {
    # Keep the per-class hook backups; delete only what the zip put there.
    Get-ChildItem $Dest -File | Remove-Item -Force
    Write-Host "Removed COM Spy files from $Dest (kept $Dest\backup)"
}
