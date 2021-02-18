#include "Interrupts.h"


void PageFault_handler( struct interrupt_frame* frame )
{
	GlobalRenderer->BasicPrint( "Page Fault!" );

	while( true );
}

