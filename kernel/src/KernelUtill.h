//Kiwi KrnlUtils header

#pragma once

#include <stdint.h>
#include "cstr.h"

#include "BasicRenderer.h"
#include "efiMemory.h"
#include "Memory.h"
#include "Bitmap.h"
#include "Paging/PageFrameAllocator.h"
#include "Paging/PageTableManager.h"
#include "UserInput/Mouse.h"
#include "Time/RTC.h"
#include "Runtime/Asserts.h"

struct BootInfo
{
	Framebuffer* framebuffer;
	PSF1_FONT* PSF1_Font;
	EFI_MEMORY_DESCRIPTOR* m_Map;
	uint64_t m_MapSize;
	uint64_t m_MapDescSize;
	const char* magic[10];
};


extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;


struct KernelInfo
{
	PageTableManager* pageTableManager;
};

KernelInfo InitKernel( BootInfo* bootInfo );
