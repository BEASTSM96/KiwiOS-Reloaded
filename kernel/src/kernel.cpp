#include <stdint.h>
#include "cstr.h"

#include "BasicRenderer.h"
#include "efiMemory.h"
#include "Memory.h"
#include "Bitmap.h"
#include "PageFrameAllocator.h"

struct BootInfo
{
	Framebuffer* framebuffer;
	PSF1_FONT* PSF1_Font;
	EFI_MEMORY_DESCRIPTOR* m_Map;
	uint64_t m_MapSize;
	uint64_t m_MapDescSize;

};


extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

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

		PageFrameAllocator alloc;
		alloc.ReadEFIMemoryMap( bootInfo->m_Map, bootInfo->m_MapSize, bootInfo->m_MapDescSize );

		uint64_t kernelSize = ( uint64_t )&_KernelEnd - ( uint64_t )&_KernelStart;
		uint64_t kernelPages = ( uint64_t )kernelSize / 4096 + 1;

		alloc.LockPages( &_KernelStart, kernelPages );

		basicRenderer.CursorPosition ={ 0, basicRenderer.CursorPosition.Y + 16 };
		basicRenderer.BasicPrint( "Free RAM: " );
		basicRenderer.BasicPrint( to_string(alloc.GetFreeRAM()) );
		basicRenderer.BasicPrint( " KB" );
		basicRenderer.CursorPosition ={ 0, basicRenderer.CursorPosition.Y + 16 };
		basicRenderer.BasicPrint( "Used RAM: " );
		basicRenderer.BasicPrint( to_string(alloc.GetUsedRAM()) );
		basicRenderer.BasicPrint( " KB" );
		basicRenderer.CursorPosition ={ 0, basicRenderer.CursorPosition.Y + 16 };
		basicRenderer.BasicPrint( "Reserved RAM: " );
		basicRenderer.BasicPrint( to_string( alloc.GetReservedRAM() ) );
		basicRenderer.BasicPrint( " KB" );
		basicRenderer.CursorPosition ={ 0, basicRenderer.CursorPosition.Y + 16 };


		return;
	}

};