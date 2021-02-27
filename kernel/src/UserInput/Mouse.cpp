#include "Mouse.h"

uint8_t MousePointer[] =
{
	0b11111111, 0b11100000,
	0b11111111, 0b10000000,
	0b11111110, 0b00000000,
	0b11111100, 0b00000000,
	0b11111000, 0b00000000,
	0b11110000, 0b00000000,
	0b11100000, 0b00000000,
	0b11000000, 0b00000000,
	0b11000000, 0b00000000,
	0b10000000, 0b00000000,
	0b10000000, 0b00000000,
	0b00000000, 0b00000000,
	0b00000000, 0b00000000,
	0b00000000, 0b00000000,
	0b00000000, 0b00000000,
	0b00000000, 0b00000000,
};

void MouseWait() 
{
	uint64_t timeout = 10000000;
	while( timeout-- )
	{
		if ((inb(0x64) & 0b10) == 0)
		{
			return;
		}
	}
}

void MouseWaitInput()
{
	uint64_t timeout = 10000000;
	while( timeout-- )
	{
		if(inb( 0x64 ) & 0b1 )
		{
			return;
		}
	}
}

void MouseWrite(uint8_t val) 
{
	MouseWait();
	outb( 0x64, 0xD4 );
	MouseWait();
	outb( 0x60, val );
}

uint8_t MouseRead() 
{
	MouseWaitInput();
	return inb( 0x60 );
}

uint8_t mouseCycle = 0;
uint8_t mousePacket[ 4 ];
bool mousePacketReady = false;
POINT MousePosition;
POINT MousePositionOld;

void HandlePS2Mouse(uint8_t data) 
{

	switch( mouseCycle )
	{
		case 0:
			if(mousePacketReady) break;
			if(data & 0b00001000 == 0 ) break;
			mousePacket[ 0 ] = data;
			mouseCycle++;
			break;

		case 1:
			if( mousePacketReady ) break;
			mousePacket[ 1 ] = data;
			mouseCycle++;
			break;

		case 2:
			if( mousePacketReady ) break;
			mousePacket[ 2 ] = data;
			mousePacketReady = true;
			mouseCycle = 0;
			break;

		default:
			break;
	}

}

void ProcessMousePacket()
{
	if( !mousePacketReady ) return;

	bool xNegative, yNegative, xOverflow, yOverflow;

	if( mousePacket[ 0 ] & PS2XSign )
	{
		xNegative = true;
	}
	else xNegative = false;

	if( mousePacket[ 0 ] & PS2YSign )
	{
		yNegative = true;
	}
	else yNegative = false;

	if( mousePacket[ 0 ] & PS2XOverflow )
	{
		xOverflow = true;
	}
	else xOverflow = false;

	if( mousePacket[ 0 ] & PS2YOverflow )
	{
		yOverflow = true;
	}
	else yOverflow = false;

	if( !xNegative )
	{
		MousePosition.X += mousePacket[ 1 ];
		if( xOverflow )
		{
			MousePosition.X += 255;
		}
	}
	else
	{
		mousePacket[ 1 ] = 256 - mousePacket[ 1 ];
		MousePosition.X -= mousePacket[ 1 ];
		if( xOverflow )
		{
			MousePosition.X -= 255;
		}
	}

	if( !yNegative )
	{
		MousePosition.Y -= mousePacket[ 2 ];
		if( yOverflow )
		{
			MousePosition.Y -= 255;
		}
	}
	else
	{
		mousePacket[ 2 ] = 256 - mousePacket[ 2 ];
		MousePosition.Y += mousePacket[ 2 ];
		if( yOverflow )
		{
			MousePosition.Y += 255;
		}
	}

	if( MousePosition.X < 0 ) MousePosition.X = 0;
	if( MousePosition.X > GlobalRenderer->TargetFramebuffer->Width - 1 ) MousePosition.X = GlobalRenderer->TargetFramebuffer->Width - 1;

	if( MousePosition.Y < 0 ) MousePosition.Y = 0;
	if( MousePosition.Y > GlobalRenderer->TargetFramebuffer->Height - 1 ) MousePosition.Y = GlobalRenderer->TargetFramebuffer->Height - 1;

	GlobalRenderer->ClearMouseCursor( MousePointer, MousePositionOld );
	GlobalRenderer->DrawOverlayMouseCursor( MousePointer, MousePosition, 0xffffffff );

	mousePacketReady = false;
	MousePositionOld = MousePosition;
}

void InitPS2Mouse()
{
	outb( 0x64, 0xA8 );
	MouseWait();
	outb( 0x64, 0x20 );
	MouseWaitInput();
	uint8_t status = inb(0x60);
	status |= 0b10;
	MouseWait();
	outb( 0x64, 0x60 );
	MouseWait();
	outb( 0x60, status );

	MouseWrite( 0xF6 );
	MouseRead();

	MouseWrite( 0xF4 );
	MouseRead();
}

