#include "BasicRenderer.h"

BasicRenderer* GlobalRenderer;

BasicRenderer::BasicRenderer( Framebuffer* targetFramebuffer, PSF1_FONT* PSF1_font, unsigned int colour )
{
	TargetFramebuffer = targetFramebuffer;

	if( colour == 0 )
		Colour = 0xffffffff;
	else
		Colour = colour;

	PSF1_Font = PSF1_font;
	CursorPosition ={ 0,0 };
}

void BasicRenderer::PutChar( char chr, unsigned int xOff, unsigned int yOff )
{
	unsigned int* pixPtr = ( unsigned int* )TargetFramebuffer->BaseAddress;
	char* fontPtr = ( char* )PSF1_Font->GlyphBugffer + ( chr * PSF1_Font->PSF1Header->CharSize );
	for( unsigned long y = yOff; y < yOff + 16; y++ )
	{
		for( unsigned long x = xOff; x < xOff + 8; x++ )
		{
			if( ( *fontPtr & ( 0b10000000 >> ( x - xOff ) ) ) > 0 )
			{
				*( unsigned int* )( pixPtr + x + ( y * TargetFramebuffer->PixelsPerScanLine ) ) = Colour;
			}

		}
		fontPtr++;
	}
}

void BasicRenderer::PutChar( char chr )
{
	PutChar( chr, CursorPosition.X, CursorPosition.Y );
	CursorPosition.X += 8;
	if (CursorPosition.X + 8 > TargetFramebuffer->Width)
	{
		Next();
	}
}

void BasicRenderer::Clear()
{
	uint64_t fbBase = ( uint64_t )TargetFramebuffer->BaseAddress;
	uint64_t bytesPerScanline = TargetFramebuffer->PixelsPerScanLine * 4;
	uint64_t fbHeight = TargetFramebuffer->Height;
	uint64_t fbSize = TargetFramebuffer->BufferSize;

	for( int verticalScanline = 0; verticalScanline < fbHeight; verticalScanline++ )
	{
		uint64_t pixPtrBase = fbBase + ( bytesPerScanline * verticalScanline );
		for( uint32_t* pixPtr = ( uint32_t* )pixPtrBase; pixPtr < ( uint32_t* )( pixPtrBase + bytesPerScanline ); pixPtr++ )
		{
			*pixPtr = ClearColor;
		}
	}

}

void BasicRenderer::ClearChar()
{
	if( CursorPosition.X == 0 )
	{
		CursorPosition.X = TargetFramebuffer->Width;
		CursorPosition.Y -= 16;
		if( CursorPosition.Y < 0 ) CursorPosition.Y = 0;
	}

	unsigned int xOff = CursorPosition.X;
	unsigned int yOff = CursorPosition.Y;

	unsigned int* pixPtr = ( unsigned int* )TargetFramebuffer->BaseAddress;
	for( unsigned long y = yOff; y < yOff + 16; y++ )
	{
		for( unsigned long x = xOff - 8; x < xOff; x++ )
		{
			*( unsigned int* )( pixPtr + x + ( y * TargetFramebuffer->PixelsPerScanLine ) ) = ClearColor;
		}
	}

	CursorPosition.X -= 8;

	if( CursorPosition.X < 0 )
	{
		CursorPosition.X = TargetFramebuffer->Width;
		CursorPosition.Y -= 16;
		if( CursorPosition.Y < 0 ) CursorPosition.Y = 0;
	}
}

void BasicRenderer::Next()
{
	CursorPosition.X = 0;
	CursorPosition.Y += 16;
}

void BasicRenderer::PutPixel( uint32_t x, uint32_t y, uint32_t color )
{
	*( uint32_t* )( ( uint64_t )TargetFramebuffer->BaseAddress + ( x * 4 ) + ( y * TargetFramebuffer->PixelsPerScanLine * 4 ) ) = color;
}

uint32_t BasicRenderer::GetPixel( uint32_t x, uint32_t y )
{
	return *( uint32_t* )( ( uint64_t )TargetFramebuffer->BaseAddress + ( x * 4 ) + ( y * TargetFramebuffer->PixelsPerScanLine * 4 ) );
}

void BasicRenderer::ClearMouseCursor( uint8_t* mouseCursor, POINT pos )
{
	if( !MouseDrawn ) return;

	int xMax = 16;
	int yMax = 16;
	int differenceX = TargetFramebuffer->Width - pos.X;
	int differenceY = TargetFramebuffer->Height - pos.Y;

	if( differenceX < 16 ) xMax = differenceX;
	if( differenceY < 16 ) yMax = differenceY;

	for( int y = 0; y < yMax; y++ )
	{
		for( int x = 0; x < xMax; x++ )
		{
			int bit = y * 16 + x;
			int byte = bit / 8;
			if( ( mouseCursor[ byte ] & ( 0b10000000 >> ( x % 8 ) ) ) )
			{
				if( GetPixel( pos.X + x, pos.Y + y ) == MouseCursorBufferAfter[ x + y * 16 ] )
				{
					PutPixel( pos.X + x, pos.Y + y, MouseCursorBuffer[ x + y * 16 ] );
				}
			}
		}
	}
}

void BasicRenderer::DrawOverlayMouseCursor( uint8_t* mouseCursor, POINT pos, uint32_t colour )
{
	int xMax = 16;
	int yMax = 16;
	int diffX = TargetFramebuffer->Width - pos.X;
	int diffY = TargetFramebuffer->Height - pos.Y;

	if( diffX < 16 ) xMax = diffX;
	if( diffY < 16 ) yMax = diffY;

	for( int y = 0; y < yMax; y++ )
	{
		for( int x = 0; x < xMax; x++ )
		{
			int bit = y * 16 + x;
			int byte = bit / 8;
			if( ( mouseCursor[ byte ] & ( 0b10000000 >> ( x % 8 ) ) ) )
			{
				MouseCursorBuffer[ x + y * 16 ] = GetPixel( pos.X + x, pos.Y + y );
				PutPixel( pos.X + x, pos.Y + y, colour );
				MouseCursorBufferAfter[ x + y * 16 ] = GetPixel( pos.X + x, pos.Y + y );
			}
		}
	}
	MouseDrawn = true;
}

void BasicRenderer::BasicPrint( const char* str )
{
	char* chr = (char*)str;
	while( *chr != 0 )
	{
		PutChar( *chr, CursorPosition.X, CursorPosition.Y );
		CursorPosition.X += 8;
		if (CursorPosition.X + 8 > TargetFramebuffer->Width)
		{
			CursorPosition.X = 0;
			CursorPosition.Y += 16;
		}
		chr++;
	}

}