//Kiwi Asserts header

#pragma once

#include "../Panic.h"

#define ASSERT(x, msg, file, line, time) if(!(x)) { PanicInfo panicInfo; panicInfo.Reason = msg; panicInfo.File = file; panicInfo.Line = line; panicInfo.Time = time; PanicAssert(panicInfo); }