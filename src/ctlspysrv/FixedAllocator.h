/////////////////////////////////////////////////////////////
// FixedAllocator.h - Generic Delegator Component
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
#pragma once

class FixedAllocator
{
public:
	bool Startup( int cbElement, int cElemsPerBlock );
	void Shutdown();

	void* Alloc();
	void Free( void* );

private:
	// This structure prefixes each block we alloc
	struct Block
	{
		Block*	m_pNext;	// pointer to the next block
		void*	m_pFree;	// head of free list
		// managed elements start here...
	};
	Block* _allocBlock();

	int		m_cbElement;		// size of an element
	int		m_cElemsPerBlock;	// elements per block
	int		m_cbBlock;			// size * elements (cached for efficiency)
	Block*	m_pHead;
};

