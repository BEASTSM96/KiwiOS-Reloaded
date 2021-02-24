#include "KernelUtill.h"

extern "C" void _start( BootInfo* bootInfo )
{
	KernelInfo kernelInfo = InitKernel( bootInfo );
	PageTableManager* pageTableManager = kernelInfo.pageTableManager;

	GlobalRenderer->BasicPrint( "Kernel Loaded!" );

	ASSERT( false, "test" );

	while( true ) 
	{
		ProcessMousePacket();
	}

	while( true );
}
