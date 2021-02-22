#include "PageFrameAllocator.h"

uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;
bool Init = false;
PageFrameAllocator GlobalAllocator;
uint64_t pageBitmapIndex = 0;

void PageFrameAllocator::ReadEFIMemoryMap( EFI_MEMORY_DESCRIPTOR* map, size_t MapSize, size_t MapDescSize )
{
	if( Init )
		return;

	Init = true;

	uint64_t mMapEntries = MapSize / MapDescSize;

	void* largestFreeMemSeg = NULL;
	size_t largestFreeMemSegSize = 0;

	for( int i = 0; i < mMapEntries; i++ )
	{
		EFI_MEMORY_DESCRIPTOR* desc = ( EFI_MEMORY_DESCRIPTOR* )( ( uint64_t )map + ( i * MapDescSize ) );
		if( desc->type == 7 )
		{ // type = EfiConventionalMemory
			if( desc->numPages * 4096 > largestFreeMemSegSize )
			{
				largestFreeMemSeg = desc->physAddr;
				largestFreeMemSegSize = desc->numPages * 4096;
			}
		}
	}

	uint64_t memorySize = GetMemorySize( map, mMapEntries, MapDescSize );
	freeMemory = memorySize;
	uint64_t bitmapSize = memorySize / 4096 / 8 + 1;

	InitBitmap( bitmapSize, largestFreeMemSeg );

	LockPages( PageBitmap.Buffer, PageBitmap.Size / 4096 + 1 );

	for( int i = 0; i < mMapEntries; i++ )
	{
		EFI_MEMORY_DESCRIPTOR* desc = ( EFI_MEMORY_DESCRIPTOR* )( ( uint64_t )map + ( i * MapDescSize ) );
		if( desc->type != 7 )
		{ // not efiConventionalMemory
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
	if(PageBitmap.Set( index, false )) 
	{
		freeMemory += 4096;
		usedMemory -= 4096;
		if( pageBitmapIndex > index ) pageBitmapIndex = index;
	}
}

void PageFrameAllocator::FreePages( void* addr, uint64_t pageCount )
{
	for( int t = 0; t < pageCount; t++ )
	{
		FreePage( ( void* )( ( uint64_t )addr + ( t * 4096 ) ) );
	}
}

void PageFrameAllocator::LockPage( void* addr )
{
	uint64_t index = ( uint64_t )addr / 4096;
	if( PageBitmap[ index ] == true ) return;
	if( PageBitmap.Set( index, true )) 
	{
		freeMemory -= 4096;
		usedMemory += 4096;
	}
}

void PageFrameAllocator::LockPages( void* addr, uint64_t pageCount )
{
	for( int t = 0; t < pageCount; t++ )
	{
		LockPage( ( void* )( ( uint64_t )addr + ( t * 4096 ) ) );
	}
}

void PageFrameAllocator::ReservePage( void* addr )
{
	uint64_t index = ( uint64_t )addr / 4096;
	if( PageBitmap[ index ] == true ) return;
	if(PageBitmap.Set( index, true )) 
	{
		freeMemory -= 4096;
		reservedMemory += 4096;
	}
}

void PageFrameAllocator::ReservePages( void* addr, uint64_t pageCount )
{
	for( int t = 0; t < pageCount; t++ )
	{
		ReservePage( ( void* )( ( uint64_t )addr + ( t * 4096 ) ) );
	}
}

void PageFrameAllocator::UnreservePage( void* addr )
{
	uint64_t index = ( uint64_t )addr / 4096;
	if( PageBitmap[ index ] == false ) return;
	if(PageBitmap.Set( index, false )) 
	{
		freeMemory += 4096;
		reservedMemory -= 4096;
		if( pageBitmapIndex > index ) pageBitmapIndex = index;
	}
}

void PageFrameAllocator::UnreservePages( void* addr, uint64_t pageCount )
{
	for( int t = 0; t < pageCount; t++ )
	{
		UnreservePage( ( void* )( ( uint64_t )addr + ( t * 4096 ) ) );
	}
}

void* PageFrameAllocator::RequestPage()
{
	for( ; pageBitmapIndex < PageBitmap.Size * 8; pageBitmapIndex++ )
	{
		if( PageBitmap[ pageBitmapIndex ] == true ) continue;
		LockPage( ( void* )( pageBitmapIndex * 4096 ) );
		return ( void* )( pageBitmapIndex * 4096 );
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