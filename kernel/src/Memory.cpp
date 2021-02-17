#include "Memory.h"


uint64_t GetMemorySize( EFI_MEMORY_DESCRIPTOR* map, uint64_t MapEntries, uint64_t MapDescSize )
{
	static uint64_t memorySizeBytes = 0;
	if( memorySizeBytes > 0 )
		return memorySizeBytes;

	for( int i = 0; i < MapEntries; i++ )
	{
		EFI_MEMORY_DESCRIPTOR* desc = ( EFI_MEMORY_DESCRIPTOR* )( ( uint64_t )map + ( i * MapDescSize ) );
		memorySizeBytes += desc->numPages * 4096;
	}

	return memorySizeBytes;

}
