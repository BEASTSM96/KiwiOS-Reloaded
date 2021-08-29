//Kiwi IR header

#pragma once

#include "../BasicRenderer.h"
#include "../IO.h"
#include "../UserInput/Mouse.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

#define PIC_EOI 0x20

#define _IRQ __attribute__( ( interrupt ) )
#define _IRQ_X(X) __attribute__( ( interrupt ) ) X


namespace Interupts {
	void Enable();
	void Disable();
}

struct interrupt_frame;
_IRQ_X(void) PageFault_handler                          ( interrupt_frame* frame );
_IRQ_X(void) DivZero_handler                            ( interrupt_frame* frame );
_IRQ_X(void) DoubleFault_handler                        ( interrupt_frame* frame );
_IRQ_X(void) GPFault_handler                            ( interrupt_frame* frame );
_IRQ_X(void) Keyboard_Interrupt_handler                 ( interrupt_frame* frame );
_IRQ_X(void) MouseInterrupt_handler                     ( interrupt_frame* frame );
_IRQ_X(void) Non_Maskable_Interrupt_handler             ( interrupt_frame* frame );
_IRQ_X(void) Breakpoint_handler                         ( interrupt_frame* frame );
_IRQ_X(void) Overflow_handler                           ( interrupt_frame* frame );
_IRQ_X(void) Bound_Range_Exceeded_handler               ( interrupt_frame* frame );
_IRQ_X(void) Invalid_Opcode_handler                     ( interrupt_frame* frame );
_IRQ_X(void) Device_Not_Available_handler               ( interrupt_frame* frame );
_IRQ_X(void) Invalid_TSS_handler                        ( interrupt_frame* frame );
_IRQ_X(void) Segment_Not_Present_handler                ( interrupt_frame* frame );
_IRQ_X(void) StackSegment_Fault_handler                 ( interrupt_frame* frame );
_IRQ_X(void) x87_FloatingPoint_Exception_handler        ( interrupt_frame* frame );
_IRQ_X(void) Alignment_Check_handler                    ( interrupt_frame* frame );
_IRQ_X(void) Machine_Check_handler                     	( interrupt_frame* frame );
_IRQ_X(void) SIMD_Floating_Point_Exception_handler      ( interrupt_frame* frame );
_IRQ_X(void) Virtualization_Exception_handler           ( interrupt_frame* frame );
_IRQ_X(void) Security_Exception_handler                 ( interrupt_frame* frame );
_IRQ_X(void) Triple_Fault_handler                       ( interrupt_frame* frame );

void RemapPic();
void PIC_EndMaster();
void PIC_EndSalve();