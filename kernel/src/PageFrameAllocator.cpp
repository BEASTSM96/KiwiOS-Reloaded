#include "PageFrameAllocator.h"

uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;
bool Init = false;

void PageFrameAllocator::ReadEFIMemoryMap( EFI_MEMORY_DESCRIPTOR* map, size_t MapSize, size_t MapDescSize )
{
	if( Init )
		return;

	Init = true;

	uint64_t mapEntries = MapSize / MapDescSize;

	void* largestFreeMemSeg = NULL;
	size_t largestFreeMemSegSize = 0;

	for( int i = 0; i < mapEntries; i++ )
	{
		EFI_MEMORY_DESCRIPTOR* desc = ( EFI_MEMORY_DESCRIPTOR* )( ( uint64_t )map + ( i * MapDescSize ) );
		if(desc->type == 7)
		{
			if (desc->numPages * 4096 > largestFreeMemSegSize )
			{
				largestFreeMemSeg = desc->physAddr;
				largestFreeMemSegSize = desc->numPages * 4096;
			}
		}
	}

	uint64_t memorySize = GetMemorySize( map, mapEntries, MapDescSize );

	freeMemory = memorySize;
	uint64_t bitmapSize = memorySize / 4096 / 8 + 1;

	InitBitmap( bitmapSize, largestFreeMemSeg );
	LockPages( &bitmapSize, PageBitmap.Size / 4096 / 8 + 1 );

	for( int i = 0; i < mapEntries; i++ )
	{
		EFI_MEMORY_DESCRIPTOR* desc = ( EFI_MEMORY_DESCRIPTOR* )( ( uint64_t )map + ( i * MapDescSize ) );
		if(desc->type != 7)
		{
			ReservePages( desc->physAddr, desc->numPages );
		}
	}

}

void PageFrameAllocator::InitBitmap( size_t bitmapSize, void* bufferAddr )
{
	PageBitmap.Size = bitmapSize;
	PageBitmap.Buffer = ( uint8_t* )bufferAddr;
	for( int i = 0; i < bitmapSize; i++ )
	{
		*( uint8_t* )( PageBitmap.Buffer + i ) = 0;
	}

}

void PageFrameAllocator::FreePage( void* addr )
{
	uint64_t index = ( uint64_t )addr / 4096;
	if( PageBitmap[ index ] == false ) return;
	PageBitmap.Set( index, false );
	freeMemory += 4096;
	usedMemory -= 4096;
}

void PageFrameAllocator::FreePages( void* addr, uint64_t pageCount )
{
	for( int i = 0; i < pageCount; i++ )
	{
		FreePage( ( void* )( ( uint64_t )addr + ( i + 4096 ) ) );
	}
}

void PageFrameAllocator::LockPage( void* addr )
{
	uint64_t index = ( uint64_t )addr / 4096;
	if( PageBitmap[ index ] == true ) return;
	PageBitmap.Set( index, true );
	freeMemory -= 4096;
	usedMemory += 4096;
}

void PageFrameAllocator::LockPages( void* addr, uint64_t pageCount )
{
	for( int i = 0; i < pageCount; i++ )
	{
		LockPage( ( void* )( ( uint64_t )addr + ( i + 4096 ) ) );
	}
}

void PageFrameAllocator::ReservePage( void* addr )
{
	uint64_t index = ( uint64_t )addr / 4096;
	if( PageBitmap[ index ] == true ) return;
	PageBitmap.Set( index, true );
	freeMemory -= 4096;
	reservedMemory += 4096;
}

void PageFrameAllocator::ReservePages( void* addr, uint64_t pageCount )
{
	for( int i = 0; i < pageCount; i++ )
	{
		ReservePage( ( void* )( ( uint64_t )addr + ( i + 4096 ) ) );
	}
}

void PageFrameAllocator::UnreservePage( void* addr )
{
	uint64_t index = ( uint64_t )addr / 4096;
	if( PageBitmap[ index ] == true ) return;
	PageBitmap.Set( index, true );
	freeMemory -= 4096;
	reservedMemory += 4096;
}

void PageFrameAllocator::UnreservePages( void* addr, uint64_t pageCount )
{
	for( int i = 0; i < pageCount; i++ )
	{
		UnreservePage( ( void* )( ( uint64_t )addr + ( i + 4096 ) ) );
	}
}

void* PageFrameAllocator::RequestPage()
{
	for (uint64_t index = 0; index < PageBitmap.Size * 8; index++)
	{
		if( PageBitmap[ index ] == true ) continue;
		LockPage( ( void* )( index * 4096 ) );
		return ( void* )( index * 4096 );
	}

	return NULL;
}


uint64_t PageFrameAllocator::GetFreeRAM()
{
	return freeMemory;
}

uint64_t PageFrameAllocator::GetUsedRAM()
{
	return usedMemory;
}

uint64_t PageFrameAllocator::GetReservedRAM()
{
	return reservedMemory;
}