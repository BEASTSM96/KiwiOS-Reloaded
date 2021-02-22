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

	POINT CursorPosition;
	PSF1_FONT* PSF1_Font;
	Framebuffer* TargetFramebuffer;
	unsigned int Colour;
	unsigned int ClearColor;

protected:

private:
};

extern BasicRenderer* GlobalRenderer;