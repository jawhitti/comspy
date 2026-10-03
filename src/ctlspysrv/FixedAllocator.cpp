/////////////////////////////////////////////////////////////
// FixedAllocator.cpp - COMSpy
//
// Copyright 1998, Keith Brown
//
// This is a very simple implementation of a fixed-size
// buffer allocator. Nothing that hasn't been done before,
// but the use of this allocator makes it less likely that
// any given call will fail due to fragmented linear address
// space. It's also more efficient than calling CoTaskMemAlloc
// for each postprocessed call. Note that the allocator
// does not perform synchronization. It is assumed that this
// is done externally.
/////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "FixedAllocator.h"

bool FixedAllocator::Startup( int cbElement, int cElemsPerBlock )
{
	// we need to store a pointer inside each free item,
	// so this allocator is only efficient in space for elements
	// that are at least as big as a pointer.
	if ( cbElement < sizeof( void* ) )
		return false;

	m_cbElement = cbElement;
	m_cElemsPerBlock = cElemsPerBlock;
	m_cbBlock = sizeof( Block ) + ( cbElement * cElemsPerBlock );
	m_pHead = _allocBlock();
	return 0 != m_pHead;
}

void FixedAllocator::Shutdown()
{
	Block* pBlock = m_pHead;
	while ( pBlock )
	{
		Block* pTail = pBlock->m_pNext;
		delete pBlock;
		pBlock = pTail;
	}
}

void* FixedAllocator::Alloc()
{
	Block* pBlock = m_pHead;
	void* pFree = m_pHead->m_pFree;

	// search for the first block with a free element
	while ( !pFree && pBlock )
	{
		pBlock = pBlock->m_pNext;
		if ( pBlock )
			pFree = pBlock->m_pFree;
	}

	// alloc a new block if necessary
	if ( !pBlock )
	{
		pBlock = _allocBlock();
		if ( pBlock )
		{
			pBlock->m_pNext = m_pHead;
			m_pHead = pBlock;
			pFree = pBlock->m_pFree;
		}
	}
	
	// maintain the free list
	if ( pFree )
		pBlock->m_pFree = *reinterpret_cast<void**>( pFree );
	return pFree;
}

void FixedAllocator::Free( void* p )
{
	Block* pBlock = m_pHead;
	while ( pBlock )
	{
		BYTE* begin = reinterpret_cast<BYTE*>( pBlock );
		if ( ( p > begin ) && ( p < ( begin + m_cbBlock ) ) )
		{
			*reinterpret_cast<void**>( p ) = pBlock->m_pFree;
			pBlock->m_pFree = p;
			break;
		}
		pBlock = pBlock->m_pNext;
	}
}

FixedAllocator::Block* FixedAllocator::_allocBlock()
{
	Block* pBlock = reinterpret_cast<Block*>( malloc( m_cbBlock ) );
	if ( pBlock )
	{
		ZeroMemory( pBlock, m_cbBlock );
		BYTE* end = reinterpret_cast<BYTE*>( pBlock ) + m_cbBlock;
		BYTE* it  = reinterpret_cast<BYTE*>( pBlock ) + sizeof( Block );
		void* pPrev = 0;
		while ( it != end )
		{
			*reinterpret_cast<void**>( it ) = pPrev;
			pPrev = it;
			it += m_cbElement;
		}
		pBlock->m_pFree = pPrev;
	}
	return pBlock;
}
