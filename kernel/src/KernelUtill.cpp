#include "KernelUtill.h"
#include "GDT/GDT.h"
#include "Interrupts/IDT.h"
#include "Interrupts/Interrupts.h"

KernelInfo kernelInfo;
PageTableManager pageTableManager = NULL;

void PrepareMemory( BootInfo* bootInfo )
{
	uint64_t mMapEntries = bootInfo->m_MapSize / bootInfo->m_MapDescSize;

	GlobalAllocator = PageFrameAllocator();
	GlobalAllocator.ReadEFIMemoryMap( bootInfo->m_Map, bootInfo->m_MapSize, bootInfo->m_MapDescSize );

	uint64_t kernelSize = ( uint64_t )&_KernelEnd - ( uint64_t )&_KernelStart;
	uint64_t kernelPages = ( uint64_t )kernelSize / 4096 + 1;

	GlobalAllocator.LockPages( &_KernelStart, kernelPages );

	PageTable* PML4 = ( PageTable* )GlobalAllocator.RequestPage();
	memset( PML4, 0, 0x1000 );

	pageTableManager = PageTableManager( PML4 );

	for( uint64_t t = 0; t < GetMemorySize( bootInfo->m_Map, mMapEntries, bootInfo->m_MapDescSize ); t+= 0x1000 )
	{
		pageTableManager.MapMemory( ( void* )t, ( void* )t );
	}

	uint64_t fbBase = ( uint64_t )bootInfo->framebuffer->BaseAddress;
	uint64_t fbSize = ( uint64_t )bootInfo->framebuffer->BufferSize + 0x1000;
	GlobalAllocator.LockPages( ( void* )fbBase, fbSize / 0x1000 + 1 );
	for( uint64_t t = fbBase; t < fbBase + fbSize; t += 4096 )
	{
		pageTableManager.MapMemory( ( void* )t, ( void* )t );
	}

	asm( "mov %0, %%cr3" : : "r" ( PML4 ) );

	kernelInfo.pageTableManager = &pageTableManager;
}

IDTR idtr;
void PrepInterrupts() 
{
	idtr.Limit = 0x0FFF;
	idtr.Offset = ( uint64_t )GlobalAllocator.RequestPage();

	IDTDescEntry* int_PageFault = ( IDTDescEntry* )( idtr.Offset + 0xE * sizeof( IDTDescEntry ) );
	int_PageFault->SetOffset( ( uint64_t )PageFault_handler );
	int_PageFault->type_attr = IDT_TA_InterruptGate;
	int_PageFault->selector = 0x08;

	asm( "lidt %0" : : "m" ( idtr ) );
}

BasicRenderer r = BasicRenderer( NULL, NULL );

KernelInfo InitKernel( BootInfo* bootInfo )
{
	r = BasicRenderer( bootInfo->framebuffer, bootInfo->PSF1_Font );
	GlobalRenderer = &r;

	GDTDescriptor gdtDescriptor;
	gdtDescriptor.Size = sizeof( GDT ) - 1;
	gdtDescriptor.Offset = ( uint64_t )&DefaultGDT;
	LoadGDT( &gdtDescriptor );

	PrepareMemory( bootInfo );

	memset( bootInfo->framebuffer->BaseAddress, 0, bootInfo->framebuffer->BufferSize );

	PrepInterrupts();

	return kernelInfo;
}
