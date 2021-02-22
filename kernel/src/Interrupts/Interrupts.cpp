#include "Interrupts.h"
#include "../Panic.h"
#include "../UserInput/Keyboard.h"

__attribute__( ( interrupt ) ) void PageFault_handler( interrupt_frame* frame )
{
	Panic( "Page Fault Detected" );

	while( true );
}

__attribute__( ( interrupt ) ) void DivZero_handler( interrupt_frame* frame )
{
	Panic( "DivZero Detected" );

	while( true );
}

__attribute__( ( interrupt ) ) void DoubleFault_handler( interrupt_frame* frame )
{
	Panic( "Double Fault Detected" );

	while( true );
}

__attribute__( ( interrupt ) ) void GPFault_handler( interrupt_frame* frame )
{
	Panic( "General Protection Fault Detected" );

	while( true );
}

__attribute__( ( interrupt ) ) void Keyboard_Interrupt_handler( interrupt_frame* frame )
{
	uint8_t scancode = inb( 0x60 );

	HandleKeyboard( scancode );

	PIC_EndMaster();
}

__attribute__( ( interrupt ) ) void MouseInterrupt_handler( interrupt_frame* frame )
{
	uint8_t mouseData = inb( 0x60 );

	HandlePS2Mouse( mouseData );

	PIC_EndSalve();
}

__attribute__( ( interrupt ) ) void Non_Maskable_Interrupt_handler( interrupt_frame* frame )
{
	Panic( "Non Maskable Interrupt Detected" );

	while( true );
}

__attribute__( ( interrupt ) ) void Breakpoint_handler( interrupt_frame* frame )
{
	Panic( "Breakpoint Detected" );

	while( true );
}

__attribute__( ( interrupt ) ) void Overflow_handler( interrupt_frame* frame )
{
	Panic( "Overflow Detected" );

	while( true );
}

__attribute__( ( interrupt ) ) void Bound_Range_Exceeded_handler( interrupt_frame* frame )
{
	Panic( "Bound Range Exceeded" );

	while( true );
}

__attribute__( ( interrupt ) ) void Invalid_Opcode_handler( interrupt_frame* frame )
{
	Panic( "Invalid Opcode Detected" );

	while( true );
}

__attribute__( ( interrupt ) ) void Device_Not_Available_handler( interrupt_frame* frame )
{
	Panic( "Device Not Available Detected" );

	while( true );
}

__attribute__( ( interrupt ) ) void Invalid_TSS_handler( interrupt_frame* frame )
{
	Panic( "Invalid TSS Detected" );

	while( true );
}

__attribute__( ( interrupt ) ) void Segment_Not_Present_handler( interrupt_frame* frame )
{
	Panic( "Segment Not Present Detected" );

	while( true );
}

__attribute__( ( interrupt ) ) void StackSegment_Fault_handler( interrupt_frame* frame )
{
	Panic( "Stack Segment Fault Detected" );

	while( true );
}

__attribute__( ( interrupt ) ) void x87_FloatingPoint_Exception_handler( interrupt_frame* frame )
{
	Panic( "x87 Floating Point Exception Detected" );

	while( true );
}

__attribute__( ( interrupt ) ) void Alignment_Check_handler( interrupt_frame* frame )
{
	Panic( "Alignment Check Detected" );

	while( true );
}

__attribute__( ( interrupt ) ) void Machine_Check_handler( interrupt_frame* frame )
{
	Panic( "Machine Check Detected" );

	while( true );
}

__attribute__( ( interrupt ) ) void SIMD_Floating_Point_Exception_handler( interrupt_frame* frame )
{
	Panic( " SIMD Floating Point Exception Detected" );

	while( true );
}

__attribute__( ( interrupt ) ) void Virtualization_Exception_handler( interrupt_frame* frame )
{
	Panic( "Virtualization Exception Detected" );

	while( true );
}

__attribute__( ( interrupt ) ) void Security_Exception_handler( interrupt_frame* frame )
{
	Panic( "Security Exception Detected" );

	while( true );
}

__attribute__( ( interrupt ) ) void Triple_Fault_handler( interrupt_frame* frame )
{
	Panic( "Triple Fault Detected" );

	while( true );
}

void RemapPic()
{
	uint8_t a1, a2;

	a1 = inb( PIC1_DATA );
	io_wait();
	a2 = inb( PIC2_DATA );
	outb( PIC1_COMMAND, ICW1_INIT | ICW1_ICW4 );
	io_wait();
	outb( PIC2_COMMAND, ICW1_INIT | ICW1_ICW4 );

	outb( PIC1_DATA, 0x20 );
	io_wait();
	outb( PIC2_DATA, 0x28 );
	io_wait();

	outb( PIC1_DATA, 4 );
	io_wait();
	outb( PIC2_DATA, 2 );
	io_wait();

	outb( PIC1_DATA, ICW4_8086 );
	io_wait();
	outb( PIC2_DATA, ICW1_ICW4 );
	io_wait();

	outb( PIC1_DATA, a1 );
	io_wait();
	outb( PIC2_DATA, a2 );
}

void PIC_EndMaster()
{
	outb( PIC1_COMMAND, PIC_EOI );
}

void PIC_EndSalve()
{
	outb( PIC2_COMMAND, PIC_EOI );
	PIC_EndMaster();
}

