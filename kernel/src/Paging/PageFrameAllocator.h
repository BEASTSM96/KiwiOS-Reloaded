//Kiwi Page frame alloc header

#pragma once

#include <stdint.h>
#include "../efiMemory.h"
#include "../Bitmap.h"
#include "../Memory.h"

class PageFrameAllocator
{
public:
	void ReadEFIMemoryMap( EFI_MEMORY_DESCRIPTOR* map, size_t MapSize, size_t MapDescSize );
	void FreePage( void* addr );
	void LockPage( void* addr );

	void FreePages( void* addr, uint64_t pageCount );
	void LockPages( void* addr, uint64_t pageCount );

	void* RequestPage();

	uint64_t GetFreeRAM();
	uint64_t GetUsedRAM();
	uint64_t GetReservedRAM();

public:
	Bitmap PageBitmap;
private:
	void InitBitmap( size_t bitmapSize, void* bufferAddr );

	void ReservePage( void* addr );
	void UnreservePage( void* addr );

	void ReservePages( void* addr, uint64_t pageCount );
	void UnreservePages( void* addr, uint64_t pageCount );
};