<#
.SYNOPSIS
    Registers (or with -Remove, unregisters) ComSpy.TestObject, a throwaway 32-bit COM object
    to spy on. It's a Windows Script Component served by scrobj.dll.

.EXAMPLE
    .\test-target.ps1
    .\test-target.ps1 -Remove
#>
param([switch]$Remove)
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'common.ps1')
Assert-Admin

$wsc   = (Resolve-Path (Join-Path $PSScriptRoot '..\test\ComSpyTest.wsc')).Path
$clsid = '{9A619258-B104-4765-A375-1A6AF4B0EB68}'

if ($Remove) {
    $k = Open-Hklm "$(Get-InprocKeyPath $clsid)"
    if ($k) {
        $hooked = $null -ne $k.GetValue('HookSavedPath'); $k.Close()
        if ($hooked) { & (Join-Path $PSScriptRoot 'unhook.ps1') ComSpy.TestObject }
    }
    $code = Invoke-Checked $Regsvr32 @('/s', '/u', '/n', "/i:`"$wsc`"", 'scrobj.dll') 'unregister ComSpy.TestObject'
} else {
    $code = Invoke-Checked $Regsvr32 @('/s', '/n', "/i:`"$wsc`"", 'scrobj.dll') 'register ComSpy.TestObject'
    $k = Open-Hklm (Get-InprocKeyPath $clsid)
    if ($k) {
        Write-Host "  32-bit InprocServer32: $($k.GetValue('')) (ThreadingModel=$($k.GetValue('ThreadingModel')))"
        $k.Close()
    } else {
        Write-Host '  but no 32-bit registration was found.' -ForegroundColor Red
    }
}
exit $code
