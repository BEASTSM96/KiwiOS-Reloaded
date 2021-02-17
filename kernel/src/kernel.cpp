#include "KernelUtill.h"

extern "C" void _start( BootInfo* bootInfo )
{
	KernelInfo kernelInfo = InitKernel( bootInfo );
	PageTableManager* pageTableManager = kernelInfo.pageTableManager;

	BasicRenderer newRenderer = BasicRenderer( bootInfo->framebuffer, bootInfo->PSF1_Font );

	memset( bootInfo->framebuffer->BaseAddress, 0, bootInfo->framebuffer->BufferSize );

	pageTableManager->MapMemory( ( void* )0x600000000, ( void* )0x80000 );

	newRenderer.BasicPrint( "Kernel Loaded!" );

	while( true );
}
