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
void SetIDTGate( void* handler, uint8_t offset, uint8_t type_attr, uint8_t selector )
{
	IDTDescEntry* interrupt = ( IDTDescEntry* )( idtr.Offset + offset * sizeof( IDTDescEntry ) );
	interrupt->SetOffset( ( uint64_t )handler );
	interrupt->type_attr = type_attr;
	interrupt->selector = selector;
}

void PrepInterrupts()
{
	idtr.Limit = 0x0FFF;
	idtr.Offset = ( uint64_t )GlobalAllocator.RequestPage();

	SetIDTGate( ( void* )PageFault_handler, 0xE, IDT_TA_InterruptGate, 0x08 );
	SetIDTGate( ( void* )DoubleFault_handler, 0x8, IDT_TA_InterruptGate, 0x08 );
	SetIDTGate( ( void* )GPFault_handler, 0xD, IDT_TA_InterruptGate, 0x08 );
	SetIDTGate( ( void* )Keyboard_Interrupt_handler, 0x21, IDT_TA_InterruptGate, 0x08 );
	SetIDTGate( ( void* )DivZero_handler, 0x0, IDT_TA_InterruptGate, 0x08 );
	SetIDTGate( ( void* )Non_Maskable_Interrupt_handler, 0x2, IDT_TA_InterruptGate, 0x08 );
	SetIDTGate( ( void* )Breakpoint_handler, 0x3, IDT_TA_InterruptGate, 0x08 );
	SetIDTGate( ( void* )Overflow_handler, 0x4, IDT_TA_InterruptGate, 0x08 );
	SetIDTGate( ( void* )Bound_Range_Exceeded_handler, 0x5, IDT_TA_InterruptGate, 0x08 );
	SetIDTGate( ( void* )Invalid_Opcode_handler, 0x6, IDT_TA_InterruptGate, 0x08 );
	SetIDTGate( ( void* )Invalid_TSS_handler, 0xA, IDT_TA_InterruptGate, 0x08 );
	SetIDTGate( ( void* )Segment_Not_Present_handler, 0xB, IDT_TA_InterruptGate, 0x08 );
	SetIDTGate( ( void* )StackSegment_Fault_handler, 0xC, IDT_TA_InterruptGate, 0x08 );
	SetIDTGate( ( void* )x87_FloatingPoint_Exception_handler, 0x10, IDT_TA_InterruptGate, 0x08 );
	SetIDTGate( ( void* )Alignment_Check_handler, 0x11, IDT_TA_InterruptGate, 0x08 );
	SetIDTGate( ( void* )Machine_Check_handler, 0x12, IDT_TA_InterruptGate, 0x08 );
	SetIDTGate( ( void* )SIMD_Floating_Point_Exception_handler, 0x13, IDT_TA_InterruptGate, 0x08 );
	SetIDTGate( ( void* )Virtualization_Exception_handler, 0x14, IDT_TA_InterruptGate, 0x08 );
	SetIDTGate( ( void* )MouseInterrupt_handler, 0x2C, IDT_TA_InterruptGate, 0x08 );

	asm( "lidt %0" : : "m" ( idtr ) );

	RemapPic();

}

BasicRenderer r = BasicRenderer( NULL, NULL );

KernelInfo InitKernel( BootInfo* bootInfo )
{
	bool allSafe = true;

	if( bootInfo->magic[ 0 ] == "M" && bootInfo->magic[ 1 ] == "A" && bootInfo->magic[ 2 ] == "G" && bootInfo->magic[ 3 ] == "I" && bootInfo->magic[ 4 ] == "C" )
	{
		bootInfo->magic[ 5 ] = " ";
		bootInfo->magic[ 6 ] = "P";
		bootInfo->magic[ 7 ] = "A";
		bootInfo->magic[ 8 ] = "S";
		bootInfo->magic[ 9 ] = "S";
		bootInfo->magic[ 10 ] = "E";
		bootInfo->magic[ 11 ] = "D";
	}

	r = BasicRenderer( bootInfo->framebuffer, bootInfo->PSF1_Font );
	GlobalRenderer = &r;

	GDTDescriptor gdtDescriptor;
	gdtDescriptor.Size = sizeof( GDT ) - 1;
	gdtDescriptor.Offset = ( uint64_t )&DefaultGDT;
	LoadGDT( &gdtDescriptor );

	PrepareMemory( bootInfo );

	memset( bootInfo->framebuffer->BaseAddress, 0, bootInfo->framebuffer->BufferSize );

	PrepInterrupts();

	ReadRTC();

	InitPS2Mouse();

	outb( PIC1_DATA, 0b11111001 );
	outb( PIC2_DATA, 0b11101111 );

	asm volatile( "sti" );

	GlobalRenderer->BasicPrint( "MAGIC PASSED" );
	GlobalRenderer->Next();

	return kernelInfo;
}
