#include <stdint.h>

#include "BasicRenderer.h"
#include "cstr.h"

extern "C"
{
	void _start( Framebuffer* framebuffer, PSF1_FONT* psf1_font )
	{
		BasicRenderer basicRenderer(framebuffer, psf1_font);
		basicRenderer.BasicPrint( "Test C++ 123 \n\r" );

		basicRenderer.BasicPrint( to_string( ( uint64_t )1234976 ) );
		basicRenderer.CursorPosition ={ 0, 16 };
		basicRenderer.BasicPrint( to_string( ( int64_t )-1234976 ) );
		basicRenderer.CursorPosition ={ 0, 32 };
		basicRenderer.BasicPrint( to_string( ( double )-13.14 ) );
		basicRenderer.CursorPosition ={ 0, 48 };
		basicRenderer.BasicPrint( to_hstring( ( uint64_t )0xF0 ) );
		basicRenderer.CursorPosition ={ 0, basicRenderer.CursorPosition.Y + 16 };
		basicRenderer.BasicPrint( to_hstring( ( uint32_t )0xF0 ) );
		basicRenderer.CursorPosition ={ 0, basicRenderer.CursorPosition.Y + 16 };
		basicRenderer.BasicPrint( to_hstring( ( uint16_t )0xF0 ) );
		basicRenderer.CursorPosition ={ 0, basicRenderer.CursorPosition.Y + 16 };
		basicRenderer.BasicPrint( to_hstring( ( uint8_t )0xF0 ) );

		return;
	}

};