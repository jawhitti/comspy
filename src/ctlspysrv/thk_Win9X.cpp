/////////////////////////////////////////////////////////////
// thk_Win9X.cpp - Generic Delegator Component
//
// Copyright 1998, Keith Brown
//
// Reverse thunking layer that makes our string handling
// routines work fast on WinNT, while supporting Win9X
// with a thunking layer.
/////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "thk_Win9X.h"

extern "C" {
	LONG (__stdcall *thk_RegOpenKeyExW)( HKEY hKey, const wchar_t* pszSubkey, DWORD options, REGSAM grfAccessMask, HKEY* phKey );
//	LONG (__stdcall *thk_RegQueryValueExW)( HKEY hKey, const wchar_t* pszValue, DWORD* pReserved, DWORD* pType, BYTE* pData, DWORD* pcbData );
	LONG (__stdcall *thk_RegCreateKeyExW)( HKEY hkey, const wchar_t* pszKey, DWORD Reserved, wchar_t* pszClass, DWORD dwOptions, REGSAM grfAccess, LPSECURITY_ATTRIBUTES psa, HKEY* phkey, DWORD* pDisposition );
	LONG (__stdcall *thk_RegSetValueExW)( HKEY hkey, const wchar_t* pszValue, DWORD, DWORD dwType, const BYTE* pbData, DWORD cbData );
	LONG (__stdcall *thk_RegDeleteKeyW)( HKEY hKey, const wchar_t* pszKey );
	DWORD (__stdcall *thk_GetModuleFileNameW)( HMODULE hModule, wchar_t* szFile, DWORD cchFile );
	void (__stdcall *thk_OutputDebugStringW)( const wchar_t* psz );
}

static LONG __stdcall win9X_RegOpenKeyExW( HKEY hKey, const wchar_t* pszSubkeyW, DWORD options,
											REGSAM grfAccessMask, HKEY* phKey )
{
    char* pszSubkeyA = 0;
	if ( pszSubkeyW )
	{
		char sz[512];
		if ( 0 == WideCharToMultiByte( CP_ACP, 0, pszSubkeyW, -1, sz, sizeof sz, 0, 0 ) )
			return GetLastError();
		pszSubkeyA = sz;		
	}
	return RegOpenKeyExA( hKey, pszSubkeyA, options, grfAccessMask, phKey );
}

/*
  // This function works great, I just discovered I didn't need it anymore
  // after Don pointed me to CoGetPSClsid, so to save some space I removed it.
static LONG __stdcall win9X_RegQueryValueExW( HKEY hKey, const wchar_t* pszValueW,
												DWORD* pReserved, DWORD* pType,
												BYTE* pData, DWORD* pcbData )
{
	DWORD t = 0;
	if ( !pType )
		pType = &t;
    char* pszValueA = 0;
	if ( pszValueW )
	{
		char sz[512];
		if ( 0 == WideCharToMultiByte( CP_ACP, 0, pszValueW, -1, sz, sizeof sz, 0, 0 ) )
			return GetLastError();
		pszValueA = sz;		
	}
	DWORD err = RegQueryValueExA( hKey, pszValueA, pReserved, pType, pData, pcbData );
	if ( !err )
	{
		switch ( *pType )
		{
			case REG_SZ:
			case REG_MULTI_SZ:
			case REG_EXPAND_SZ:
			{
				if ( pData )
				{
					// convert Ansi result to Unicode
					wchar_t sz[1024];
					int cch = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, (char*)pData, *pcbData, sz, sizeof sz / sizeof *sz );
					if ( 0 == cch )
						return GetLastError();
					const DWORD cbWideData = cch * sizeof *sz;
					if ( *pcbData < cbWideData )
					{
						*pcbData = cbWideData;
						return ERROR_MORE_DATA;
					}
					memcpy( pData, sz, cbWideData );
					*pcbData = cbWideData;
				}
				else if ( pcbData )
					*pcbData *= sizeof( wchar_t );
				break;
			}
		}
	}
	return err;
}
*/

LONG __stdcall win9X_RegCreateKeyExW( HKEY hkey, const wchar_t* pszKeyW, DWORD Reserved, wchar_t*, DWORD dwOptions, REGSAM grfAccess, LPSECURITY_ATTRIBUTES psa, HKEY* phkey, DWORD* pDisposition )
{
	if ( !pszKeyW )
		return ERROR_INVALID_PARAMETER;

	char szKeyA[512];
	if ( 0 == WideCharToMultiByte( CP_ACP, 0, pszKeyW, -1, szKeyA, sizeof szKeyA, 0, 0 ) )
		return GetLastError();
	
	return RegCreateKeyEx( hkey, szKeyA, Reserved, "", dwOptions, grfAccess, psa, phkey, pDisposition );
}

LONG __stdcall win9X_RegSetValueExW( HKEY hkey, const wchar_t* pszValueW, DWORD Reserved, DWORD dwType, const BYTE* pbDataW, DWORD cbDataW )
{
    char* pszValueA = 0;
	if ( pszValueW )
	{
		char sz[512];
		if ( 0 == WideCharToMultiByte( CP_ACP, 0, pszValueW, -1, sz, sizeof sz, 0, 0 ) )
			return GetLastError();
		pszValueA = sz;		
	}
	const BYTE* pbDataA = pbDataW;
	DWORD cbDataA = cbDataW;

	char sz[512];
	switch ( dwType )
	{
		case REG_SZ:
		case REG_MULTI_SZ:
		case REG_EXPAND_SZ:
		{
			cbDataA = WideCharToMultiByte( CP_ACP, 0, (wchar_t*)pbDataW, cbDataW / sizeof( wchar_t ), sz, sizeof sz, 0, 0 );
			if ( !cbDataA )
				return GetLastError();
			pbDataA = (BYTE*)sz;
		}
		break;
	}
	return RegSetValueExA( hkey, pszValueA, Reserved, dwType, pbDataA, cbDataA );
}

DWORD __stdcall win9X_GetModuleFileNameW( HMODULE hModule, wchar_t* szFileW, DWORD cchFileW )
{
	char szA[MAX_PATH]; // my tests indicate that this fcn doesn't inlude the null terminator in the return value
	DWORD cchA = GetModuleFileNameA( hModule, szA, sizeof szA ) + 1;
	if ( !cchA )
		return FALSE;

	return (DWORD)MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szA, cchA, szFileW, cchFileW );
}

LONG __stdcall win9X_RegDeleteKeyW( HKEY hKey, const wchar_t* pszKeyW )
{
	if ( !pszKeyW )
		return ERROR_INVALID_PARAMETER;

	char szKeyA[512];
	if ( 0 == WideCharToMultiByte( CP_ACP, 0, pszKeyW, -1, szKeyA, sizeof szKeyA, 0, 0 ) )
		return GetLastError();
	return RegDeleteKeyA( hKey, szKeyA );
}

void __stdcall win9X_OutputDebugStringW( const wchar_t* pszW )
{
	if ( !pszW )
		return;

	char pszA[1024];
	if ( 0 != WideCharToMultiByte( CP_ACP, 0, pszW, -1, pszA, sizeof pszA, 0, 0 ) )
		OutputDebugStringA( pszA );
}

void thk_Win9X::Startup()
{
    OSVERSIONINFO v;
    v.dwOSVersionInfoSize = sizeof v;
    GetVersionEx( &v );

    if ( VER_PLATFORM_WIN32_NT == v.dwPlatformId )
    {
        thk_RegOpenKeyExW		= RegOpenKeyExW;
//        thk_RegQueryValueExW	= RegQueryValueExW;
        thk_RegCreateKeyExW		= RegCreateKeyExW;
        thk_RegSetValueExW		= RegSetValueExW;
        thk_GetModuleFileNameW	= GetModuleFileNameW;
        thk_RegDeleteKeyW		= RegDeleteKeyW;
		thk_OutputDebugStringW	= OutputDebugStringW;
    }
    else
    {
        thk_RegOpenKeyExW		= win9X_RegOpenKeyExW;
//        thk_RegQueryValueExW	= win9X_RegQueryValueExW;
        thk_RegCreateKeyExW		= win9X_RegCreateKeyExW;
        thk_RegSetValueExW		= win9X_RegSetValueExW;
        thk_GetModuleFileNameW	= win9X_GetModuleFileNameW;
        thk_RegDeleteKeyW		= win9X_RegDeleteKeyW;
		thk_OutputDebugStringW	= win9X_OutputDebugStringW;
    }
}

void thk_Win9X::Shutdown()
{}
