//kiwi KBSCT header

#pragma once

#include <stdint.h>

#define LSHIFT 0x2A
#define RSHIFT 0x32
#define ENTER 0x1C
#define BACKSPACE 0x0E
#define SPACEBAR 0x39

namespace QWERTYKeyboard {

	extern const char ASCIITable[];
	char Translate( uint8_t scancode, bool uppercase );

}