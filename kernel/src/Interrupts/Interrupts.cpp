#include "Interrupts.h"
#include "../Panic.h"
#include "../UserInput/Keyboard.h"

__attribute__( ( interrupt ) ) void PageFault_handler( struct interrupt_frame* frame )
{
	Panic( "Page Fault Detected" );

	while( true );
}

__attribute__( ( interrupt ) ) void DivZero_handler( struct interrupt_frame* frame )
{
	Panic( "DivZero Detected" );

	while( true );
}

__attribute__( ( interrupt ) ) void DoubleFault_handler( struct interrupt_frame* frame )
{
	Panic( "Double Fault Detected" );

	while( true );
}

__attribute__( ( interrupt ) ) void GPFault_handler( struct interrupt_frame* frame )
{
	Panic( "General Protection Fault Detected" );

	while( true );
}

__attribute__( ( interrupt ) ) void Keyboard_Interrupt_handler( struct interrupt_frame* frame )
{
	uint8_t scancode = inb( 0x60 );

	HandleKeyboard( scancode );

	PIC_EndMaster();
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

