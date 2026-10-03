<#
.SYNOPSIS
    Unpacks the 1999 COM Spy release and registers it.

.DESCRIPTION
    Does what the original install.bat did, but with 32-bit regsvr32 called explicitly and every
    exit code reported instead of silenced. Run from an elevated PowerShell.

.EXAMPLE
    .\install.ps1
    .\install.ps1 -Dest D:\tools\comspy
#>
param(
    [string]$Dest = 'C:\comspy',
    [string]$Zip  = (Join-Path $PSScriptRoot '..\archive\comspy01.zip')
)
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'common.ps1')
Assert-Admin

Write-Host "Unpacking $Zip to $Dest"
Expand-Archive -Path $Zip -DestinationPath $Dest -Force

# No registry backup here: registering only adds keys under COM Spy's own GUIDs, and
# uninstall.ps1 removes them. hook.ps1 backs up each class it actually changes.
Write-Host 'Registering COM Spy'
$failures = 0
foreach ($dll in 'ctlspysrv.dll', 'ctlspysrvps.dll', 'debuggerps.dll') {
    if (Invoke-Checked $Regsvr32 @('/s', "`"$(Join-Path $Dest $dll)`"") "regsvr32 $dll") { $failures++ }
}
if (Invoke-Checked (Join-Path $Dest 'agent.exe') @('/RegServer') 'agent /RegServer') { $failures++ }

Write-Host 'Checking registrations'
foreach ($clsid in @($SpyClsid) + $AgentClsids) {
    $k = Open-Hklm "$Classes32\CLSID\$clsid"
    if ($k) { Write-Host "  found   $clsid ($($k.GetValue('')))"; $k.Close() }
    else    { Write-Host "  MISSING $clsid" -ForegroundColor Red; $failures++ }
}

if ($failures) {
    Write-Host "`n$failures problem(s). Run uninstall.ps1 to clean up." -ForegroundColor Red
    exit 1
}
Write-Host "`nInstalled. Next: .\hook.ps1 <ProgID>, then start $Dest\comspy.exe, then create the control in a 32-bit host."
