//Kiwi Memory Header

#pragma once

#include <stdint.h>
#include "efiMemory.h"

uint64_t GetMemorySize( EFI_MEMORY_DESCRIPTOR* map, uint64_t MapEntries, uint64_t MapDescSize );
void memset( void* start, uint8_t value, uint64_t num );
