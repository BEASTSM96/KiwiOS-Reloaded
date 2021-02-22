#include "KernelUtill.h"

extern "C" void _start( BootInfo* bootInfo )
{
	KernelInfo kernelInfo = InitKernel( bootInfo );
	PageTableManager* pageTableManager = kernelInfo.pageTableManager;

	GlobalRenderer->BasicPrint( "Kernel Loaded!" );

	while( true ) 
	{
		ProcessMousePacket();
	}

	while( true );
}
