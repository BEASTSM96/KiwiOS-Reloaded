#include "Keyboard.h"


bool LSPressed;
bool RSPressed;

void HandleKeyboard( uint8_t scancode )
{

	switch( scancode )
	{
		case LSHIFT:
			LSPressed = true;
			return;

		case LSHIFT + 0x80:
			LSPressed = false;
			return;

		case RSHIFT:
			RSPressed = true;
			return;

		case RSHIFT + 0x80:
			RSPressed = false;
			return;

		case ENTER:
			GlobalRenderer->Next();
			return;

		case SPACEBAR:
			GlobalRenderer->PutChar( ' ' );
			return;

		case BACKSPACE:
			GlobalRenderer->ClearChar();
			return;

		default:
			break;
	}
	

	char ascii = QWERTYKeyboard::Translate( scancode, LSPressed || RSPressed );

	if (ascii != 0)
	{
		GlobalRenderer->PutChar( ascii );
	}
}

