#include "KernelUtill.h"

extern "C" void _start( BootInfo* bootInfo )
{
	KernelInfo kernelInfo = InitKernel( bootInfo );
	PageTableManager* pageTableManager = kernelInfo.pageTableManager;

	GlobalRenderer->BasicPrint( "Kernel Loaded!" );

	int a = 6 / 1;

	while( true );
}
