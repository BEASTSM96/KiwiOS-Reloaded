#include <stdint.h>
#include "cstr.h"

#include "BasicRenderer.h"
#include "efiMemory.h"
#include "Memory.h"
#include "Bitmap.h"

struct BootInfo
{
	Framebuffer* framebuffer;
	PSF1_FONT* PSF1_Font;
	EFI_MEMORY_DESCRIPTOR* m_Map;
	uint64_t m_MapSize;
	uint64_t m_MapDescSize;

};

uint8_t testbuffer[ 20 ];
extern "C"
{
	void _start( BootInfo* bootInfo )
	{
		BasicRenderer basicRenderer( bootInfo->framebuffer, bootInfo->PSF1_Font );

		basicRenderer.BasicPrint( "\n===============\n\r");
		basicRenderer.BasicPrint( "\n\r ==============\n\r " );
		basicRenderer.BasicPrint( "\n\r MEMORY SIZE \n\r " );

		uint64_t MapEntries = bootInfo->m_MapSize / bootInfo->m_MapDescSize;

		basicRenderer.BasicPrint( to_string( GetMemorySize( bootInfo->m_Map, MapEntries, bootInfo->m_MapDescSize ) ) );


		for( int i = 0; i < 20; i++ )
		{
			basicRenderer.CursorPosition ={ 0, basicRenderer.CursorPosition.Y + 16 };
			basicRenderer.BasicPrint( testBitmap[ i ] ? "true" : "false" );
		}

		return;
	}

};