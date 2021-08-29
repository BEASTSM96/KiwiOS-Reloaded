//Kiwi Painc Message 

#pragma once

#include "BasicRenderer.h"
#include "Interrupts/Interrupts.h"
#include "cstr.h"

struct PanicInfo 
{
	int         Line;
	const char* Time;
	const char* File;
	const char* Reason;
};

void Panic       ( const char* panicMessage );
void PanicAssert ( PanicInfo panicInfo );