//Kiwi BasicRenderer Header

#pragma once

#include "Math.h"
#include "Framebuffer.h"
#include "SimpleFonts.h"
#include <stdint.h>

class BasicRenderer
{
public:
	BasicRenderer( Framebuffer* targetFramebuffer, PSF1_FONT* PSF1_font, unsigned int colour = 0 );

	void BasicPrint( const char* str );

	void PutChar( char chr, unsigned int xOff, unsigned int yOff );
	void PutChar( char chr );

	void Clear();
	void ClearChar();
	void Next();
	void DrawOverlayMouseCursor( uint8_t* mouseCursor, POINT pos, uint32_t colour);
	void PutPixel( uint32_t x, uint32_t y, uint32_t color );
	void ClearMouseCursor( uint8_t* mouseCursor, POINT pos );
	uint32_t GetPixel( uint32_t x, uint32_t y );

public:
	POINT CursorPosition;
	PSF1_FONT* PSF1_Font;
	Framebuffer* TargetFramebuffer;
	unsigned int Colour;
	unsigned int ClearColor;
	bool MouseDrawn;

	uint32_t MouseCursorBuffer[ 16 * 16 ];
	uint32_t MouseCursorBufferAfter[ 16 * 16 ];

protected:

private:
};

extern BasicRenderer* GlobalRenderer;