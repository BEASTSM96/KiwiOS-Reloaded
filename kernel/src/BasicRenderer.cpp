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