//Kiwi Asserts header

#pragma once

#include "../Panic.h"
#include "Runtime.h"

#define ASSERT(x, msg) if(!(x)) { Panic(msg); }