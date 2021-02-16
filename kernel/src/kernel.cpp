typedef unsigned long long size_t;

typedef struct
{
	void* BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanLine;
} Framebuffer;

typedef struct
{
	unsigned char Magic[ 2 ];
	unsigned char Mode;
	unsigned char CharSize;
} PSF1_HEADER;

typedef struct
{
	PSF1_HEADER* PSF1Header;
	void* GlyphBugffer;

} PSF1_FONT;

typedef struct
{
	unsigned int X;
	unsigned int Y;
} POINT;

void PutChar
( 
	Framebuffer* framebuffer, 
	PSF1_FONT* psf1_font, 
	unsigned int colour, 
	char chr, 
	unsigned int xOff, 
	unsigned int yOff 
)
{
	unsigned int* pixPtr = ( unsigned int* )framebuffer->BaseAddress;
	char* fontPtr = psf1_font->GlyphBugffer + ( chr * psf1_font->PSF1Header->CharSize );
	for( unsigned long y = yOff; y < yOff + 16; y++ )
	{
		for( unsigned long x = xOff; x < xOff + 8; x++ )
		{
			if( ( *fontPtr & ( 0b10000000 >> ( x - xOff ) ) ) > 0 )
			{
				*( unsigned int* )( pixPtr + x + ( y * framebuffer->PixelsPerScanLine ) ) = colour;
			}

		}
		fontPtr++;
	}
}

POINT CursorPos;
void Print
(
	Framebuffer* framebuffer,
	PSF1_FONT* psf1_font,
	unsigned int colour,
	char* str 
)
{
	unsigned int x = 0, y = 0;
	char* chr = str;
	while( *chr != 0 )
	{
		PutChar( framebuffer, psf1_font, colour, *chr, x, 0 );
		x += 8;
		chr++;
	}

}


extern "C" void _start( Framebuffer* framebuffer, PSF1_FONT* psf1_font )
{

	Print( framebuffer, psf1_font, 0xffffffff, "Hello World\n\r" );
	Print( framebuffer, psf1_font, 0xffffffff, "Hello World\n\r" );
	Print( framebuffer, psf1_font, 0xffffffff, "Hello World\n\r" );
	Print( framebuffer, psf1_font, 0xffffffff, "Hello World\n\r" );
	Print( framebuffer, psf1_font, 0xffffffff, "Hello World\n\r" );
	Print( framebuffer, psf1_font, 0xffffffff, "Hello World\n\r" );
	Print( framebuffer, psf1_font, 0xffffffff, "Hello World\n\r" );
	Print( framebuffer, psf1_font, 0xffffffff, "Hello World\n\r" );
	Print( framebuffer, psf1_font, 0xffffffff, "Hello World\n\r" );
	Print( framebuffer, psf1_font, 0xffffffff, "Hello World\n\r" );
	Print( framebuffer, psf1_font, 0xffffffff, "Hello World\n\r" );
	Print( framebuffer, psf1_font, 0xffffffff, "Hello World\n\r" );
	Print( framebuffer, psf1_font, 0xffffffff, "Hello World\n\r" );

	return;
}