#include "Runtime.h"

void Runtime::OnAssert( const char* reason )
{
	Interupts::Disable();
}
