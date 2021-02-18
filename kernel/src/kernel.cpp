#include "KernelUtill.h"

extern "C" void _start( BootInfo* bootInfo )
{
	KernelInfo kernelInfo = InitKernel( bootInfo );
	PageTableManager* pageTableManager = kernelInfo.pageTableManager;

	GlobalRenderer->BasicPrint( "Kernel Loaded!" );

	int* test = ( int* )0x80000000000;
	*test = 2;

	while( true );
}
