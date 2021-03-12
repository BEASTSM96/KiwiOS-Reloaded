#include "Panic.h"

void Panic( const char* panicMessage )
{
	GlobalRenderer->ClearColor = 0x000000FF;
	GlobalRenderer->Clear();
	GlobalRenderer->CursorPosition.X += 825;
	GlobalRenderer->Colour = 0xfffff;

	GlobalRenderer->BasicPrint( "KERNEL PANIC" );

	GlobalRenderer->Next();
	GlobalRenderer->Next();

	GlobalRenderer->CursorPosition.X = 0;
	GlobalRenderer->CursorPosition.X += 937;

	GlobalRenderer->BasicPrint( panicMessage );
}

/*
* Does not have a while(true)
*/
void PanicAssert( PanicInfo panicInfo )
{
	Interupts::Disable();

	GlobalRenderer->ClearColor = 0x000000FF;
	GlobalRenderer->Clear();

	GlobalRenderer->Colour = 0xfffff;
	GlobalRenderer->CursorPosition.X = 0;
	GlobalRenderer->BasicPrint( "KERNEL PANIC" );

	GlobalRenderer->Next();

	GlobalRenderer->BasicPrint( "DEBUG ASSERTION FAILED!" );

	GlobalRenderer->Next();
	GlobalRenderer->Next();

	GlobalRenderer->BasicPrint( "Reason : " );
	GlobalRenderer->BasicPrint( panicInfo.Reason );
	GlobalRenderer->Next();

	GlobalRenderer->BasicPrint( "File : " );
	GlobalRenderer->BasicPrint( panicInfo.File );
	GlobalRenderer->Next();

	GlobalRenderer->BasicPrint( "Line : " );
	GlobalRenderer->BasicPrint( to_string( ( uint64_t )panicInfo.Line ) );
	GlobalRenderer->Next();

	GlobalRenderer->BasicPrint( "Time : " );
	GlobalRenderer->BasicPrint( panicInfo.Time );
	GlobalRenderer->Next();
}
