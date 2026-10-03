// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__7B78CFE1_330F_11D3_8BF2_00105A6DC077__INCLUDED_)
#define AFX_STDAFX_H__7B78CFE1_330F_11D3_8BF2_00105A6DC077__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>

//_Module is extended to manage the list of debugged objects.
#include "spyModule.h"
extern CSpyModule _Module;
#include <atlcom.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__7B78CFE1_330F_11D3_8BF2_00105A6DC077__INCLUDED)
