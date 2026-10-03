/////////////////////////////////////////////////////////////
// ItfTypeInfo.h - Generic Delegator Component
//
// Copyright 1998, Keith Brown
//
/////////////////////////////////////////////////////////////
#ifndef __ITFTYPEINFO_H
#define __ITFTYPEINFO_H


struct __declspec(novtable) ItfTypeInfo
{
	// this function is not allowed to fail
	// so must already have precached all this info
	virtual DWORD GetStackSize( DWORD nMethod ) = 0;

	static HRESULT GetTypeInfo( const IID& iid, ItfTypeInfo*& pti );
    static void Startup();
	static void Shutdown();
};


#endif