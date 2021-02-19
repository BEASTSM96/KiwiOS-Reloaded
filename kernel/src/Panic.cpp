#include "Panic.h"

void Panic( const char* panicMessage )
{
	GlobalRenderer->Clear( 0x000000FF );
	GlobalRenderer->CursorPosition.X += 825;
	GlobalRenderer->Colour = 0xfffff;

	GlobalRenderer->BasicPrint( "KERNEL PANIC" );

	GlobalRenderer->Next();
	GlobalRenderer->Next();

	GlobalRenderer->CursorPosition.X = 0;
	GlobalRenderer->CursorPosition.X += 937;

	GlobalRenderer->BasicPrint( panicMessage );
}