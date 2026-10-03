/////////////////////////////////////////////////////////////
// thk_Win9X.h - Generic Delegator Component
//
// Copyright 1998, Keith Brown
//
// Reverse thunking layer that makes our string handling
// routines work fast on WinNT, while supporting Win9X
// with a thunking layer.
/////////////////////////////////////////////////////////////
#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

extern LONG (__stdcall *thk_RegOpenKeyExW)( HKEY hKey, const wchar_t* pszSubkey, DWORD options, REGSAM grfAccessMask, HKEY* phKey );
//extern LONG (__stdcall *thk_RegQueryValueExW)( HKEY hKey, const wchar_t* pszValue, DWORD* pReserved, DWORD* pType, BYTE* pData, DWORD* pcbData );
extern LONG (__stdcall *thk_RegCreateKeyExW)( HKEY hkey, const wchar_t* pszKey, DWORD Reserved, wchar_t* pszClass, DWORD dwOptions, REGSAM grfAccess, LPSECURITY_ATTRIBUTES psa, HKEY* phkey, DWORD* pDisposition );
extern LONG (__stdcall *thk_RegSetValueExW)( HKEY hkey, const wchar_t* pszValue, DWORD, DWORD dwType, const BYTE* pbData, DWORD cbData );
extern LONG (__stdcall *thk_RegDeleteKeyW)( HKEY hKey, const wchar_t* pszKey );
extern DWORD (__stdcall *thk_GetModuleFileNameW)( HMODULE hModule, wchar_t* szFile, DWORD cchFile );
extern void (__stdcall *thk_OutputDebugStringW)( const wchar_t* psz );

#ifdef __cplusplus
}
struct thk_Win9X
{
    static void Startup();
    static void Shutdown();
};
#endif // __cplusplus
