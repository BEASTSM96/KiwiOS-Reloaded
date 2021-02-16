#include <stdint.h>
#include "cstr.h"

#include "BasicRenderer.h"
#include "efiMemory.h"

struct BootInfo
{
	Framebuffer* framebuffer;
	PSF1_FONT* PSF1_Font;
	void* m_Map;
	uint64_t m_MapSize;
	uint64_t m_MapDescSize;

};


extern "C"
{
	void _start( BootInfo* bootInfo )
	{
		BasicRenderer basicRenderer( bootInfo->framebuffer, bootInfo->PSF1_Font );

		basicRenderer.BasicPrint( "\n===============\n\r");
		basicRenderer.BasicPrint( "\nTESTING MEMORY MAP...\n\r" );

		uint64_t mapEntries = bootInfo->m_MapSize / bootInfo->m_MapDescSize;

		for( int i = 0; i < mapEntries; i++ )
		{
			EFI_MEMORY_DESCRIPTOR* desc = ( EFI_MEMORY_DESCRIPTOR* )( ( uint64_t )bootInfo->m_Map + ( i * bootInfo->m_MapDescSize ) );
			basicRenderer.CursorPosition ={ 0, basicRenderer.CursorPosition.Y + 16 };
			basicRenderer.BasicPrint( EFI_MEMORY_TYPE_STRINGS[ desc->type ] );
			basicRenderer.Colour = 0xffff00ff;
			basicRenderer.BasicPrint( " " );
			basicRenderer.BasicPrint( to_string( desc->numPages * 4096 / 1024 ) );
			basicRenderer.BasicPrint( "KB\n\r" );
			basicRenderer.Colour = 0xffffffff;
		}

		return;
	}

};