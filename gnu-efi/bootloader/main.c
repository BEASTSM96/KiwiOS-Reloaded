#include <efi.h>
#include <efilib.h>
#include <elf.h>

typedef unsigned long long size_t;

typedef struct
{
	void* BassAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelPerScanline;
} Framebuffer;

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

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

Framebuffer framebuffer;
Framebuffer* InitGOP()
{
	EFI_GUID gopGUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_STATUS status;

	status = uefi_call_wrapper( BS->LocateProtocol, 3, &gopGUID, NULL, ( void** )&gop );
	if( EFI_ERROR( status ) )
	{
		Print( L"Could Not Find GOP! \n\r" );
	}
	else
	{
		Print( L"GOP Found! \n\r" );
	}

	framebuffer.BassAddress = ( void* )gop->Mode->FrameBufferBase;
	framebuffer.BufferSize = gop->Mode->FrameBufferSize;
	framebuffer.Width = gop->Mode->Info->HorizontalResolution;
	framebuffer.Height = gop->Mode->Info->VerticalResolution;
	framebuffer.PixelPerScanline = gop->Mode->Info->PixelsPerScanLine;

	return &framebuffer;
}

EFI_FILE* LoadFile( EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable )
{
	EFI_FILE* LoadedFile;

	EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
	SystemTable->BootServices->HandleProtocol( ImageHandle, &gEfiLoadedImageProtocolGuid, ( void** )&LoadedImage );

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	SystemTable->BootServices->HandleProtocol( LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, ( void** )&FileSystem );

	if( Directory == NULL )
	{
		FileSystem->OpenVolume( FileSystem, &Directory );
	}

	EFI_STATUS s = Directory->Open( Directory, &LoadedFile, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY );
	if( s != EFI_SUCCESS )
	{
		return NULL;
	}
	return LoadedFile;

}

PSF1_FONT* LoadPSF1Font( EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable )
{
	EFI_FILE* font = LoadFile( Directory, Path, ImageHandle, SystemTable );
	if( font == NULL )
		return NULL;

	PSF1_HEADER* fontHeader;
	SystemTable->BootServices->AllocatePool( EfiLoaderData, sizeof( PSF1_HEADER ), ( void** )&fontHeader );
	UINTN size = sizeof( PSF1_HEADER );
	font->Read( font, &size, fontHeader );

	if( fontHeader->Magic[ 0 ] != PSF1_MAGIC0 || fontHeader->Magic[ 1 ] != PSF1_MAGIC1 )
	{
		Print( L"Font magic was damaged!\n\r" );
		return NULL;
	}

	UINTN glyphBufferSize = fontHeader->CharSize * 256;
	if( fontHeader->Mode == 1 )
	{
		Print( L"Font CharSize is %d\n\r", fontHeader->CharSize * 512 );
		glyphBufferSize = fontHeader->CharSize * 512;
	}
	Print( L"Font CharSize is %d\n\r", fontHeader->CharSize * 256 );

	void* glyphBuffer;
	{
		font->SetPosition( font, sizeof( PSF1_HEADER ) );
		SystemTable->BootServices->AllocatePool( EfiLoaderData, glyphBufferSize, ( void** )&glyphBuffer );
		font->Read( font, &glyphBufferSize, glyphBuffer );
	}

	PSF1_FONT* retFont;
	SystemTable->BootServices->AllocatePool( EfiLoaderData, sizeof( PSF1_FONT ), ( void** )&retFont );
	retFont->PSF1Header = fontHeader;
	retFont->GlyphBugffer = glyphBuffer;
	return retFont;
}


int memcmp( const void* aptr, const void* bptr, size_t n )
{
	const unsigned char* a = aptr, * b = bptr;
	for( size_t i = 0; i < n; i++ )
	{
		if( a[ i ] < b[ i ] ) return -1;
		else if( a[ i ] > b[ i ] ) return 1;
	}
	return 0;
}

typedef struct
{
	Framebuffer* framebuffer;
	PSF1_FONT* PSF1_Font;
	EFI_MEMORY_DESCRIPTOR* m_Map;
	UINTN m_MapSize;
	UINTN m_MapDescSize;
	const char* magic[ 10 ];
} BootInfo;

EFI_STATUS efi_main( EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable )
{
	InitializeLib( ImageHandle, SystemTable );
	Print( L"efi_main \n\r" );

	EFI_FILE* Kernel = LoadFile( NULL, L"kernel.elf", ImageHandle, SystemTable );
	if( Kernel == NULL )
	{
		Print( L"Could not load krnl \n\r" );
	}
	else
	{
		Print( L"Loaded Kernel!\n\r" );
	}

	Elf64_Ehdr header;
	{
		UINTN FileInfoSize;
		EFI_FILE_INFO* FileInfo;
		Kernel->GetInfo( Kernel, &gEfiFileInfoGuid, &FileInfoSize, NULL );
		SystemTable->BootServices->AllocatePool( EfiLoaderData, FileInfoSize, ( void** )&FileInfo );
		Kernel->GetInfo( Kernel, &gEfiFileInfoGuid, &FileInfoSize, ( void** )&FileInfo );

		UINTN size = sizeof( header );
		Kernel->Read( Kernel, &size, &header );
	}

	if(
		memcmp( &header.e_ident[ EI_MAG0 ], ELFMAG, SELFMAG ) != 0 ||
		header.e_ident[ EI_CLASS ] != ELFCLASS64 ||
		header.e_ident[ EI_DATA ] != ELFDATA2LSB ||
		header.e_type != ET_EXEC ||
		header.e_machine != EM_X86_64 ||
		header.e_version != EV_CURRENT
	)
	{
		Print( L"kernel format is bad\r\n" );
	}
	else
	{
		Print( L"kernel header successfully verified\r\n" );
	}

	Elf64_Phdr* phdrs; 
	{
		Kernel->SetPosition( Kernel, header.e_phoff );
		UINTN size = header.e_phnum * header.e_phentsize;
		SystemTable->BootServices->AllocatePool( EfiLoaderData, size, ( void** )&phdrs );
		Kernel->Read(Kernel, &size, phdrs);
	}

	for(
		Elf64_Phdr* phdr = phdrs;
		( char* )phdr < ( char* )phdrs + header.e_phnum * header.e_phentsize;
		phdr = ( Elf64_Phdr* )( ( char* )phdr + header.e_phentsize )
	)
	{
		switch (phdr->p_type)
		{
			case PT_LOAD:
			{
				int pages = ( phdr->p_memsz + 0x1000 - 1 ) / 0x1000;
				Elf64_Addr segment = phdr->p_paddr;
				SystemTable->BootServices->AllocatePages( AllocateAddress, EfiLoaderData, pages, &segment );

				Kernel->SetPosition( Kernel, phdr->p_offset );
				UINTN size = phdr->p_filesz;
				Kernel->Read( Kernel, &size, ( void* )segment );
				break;
			}
		}
	}

	Print( L"Kernel Loaded \n\r" );

	
	PSF1_FONT* zapFont = LoadPSF1Font( NULL, L"zap-light16.psf", ImageHandle, SystemTable );
	if (zapFont == NULL)
	{
		Print( L"Font is not valid or not found \n\r" );
	}
	else
	{
		Print( L"Found Found. char size = %d \n\r", zapFont->PSF1Header->CharSize );
	}

	Framebuffer* buffer = InitGOP();

	Print( L"Base: 0x%x\n\r Size: 0x%x\n\r Width: %d\n\r Height: %d\n\r PixelPerScanline: %d\n\r", buffer->BassAddress, buffer->BassAddress, buffer->Width, buffer->Height, buffer->PixelPerScanline );

	EFI_MEMORY_DESCRIPTOR* Map = NULL;
	UINTN MapSize, MapKey;
	UINTN DescSize;
	UINT32 DescVer;
	{
		SystemTable->BootServices->GetMemoryMap( &MapSize, Map, &MapKey, &DescSize, &DescVer );
		SystemTable->BootServices->AllocatePool( EfiLoaderData, MapSize, ( void** )&Map );
		SystemTable->BootServices->GetMemoryMap( &MapSize, Map, &MapKey, &DescSize, &DescVer );
	}

	void ( *KrnlStart )( BootInfo* ) = ( ( __attribute__( ( sysv_abi ) ) void ( * )( BootInfo* ) ) header.e_entry );

	BootInfo bootInfo;
	bootInfo.framebuffer = buffer;
	bootInfo.PSF1_Font = zapFont;
	bootInfo.m_MapSize = MapSize;
	bootInfo.m_MapDescSize = DescSize;
	bootInfo.m_Map = Map;

	bootInfo.magic[ 0 ] = "M";
	bootInfo.magic[ 1 ] = "A";
	bootInfo.magic[ 2 ] = "G";
	bootInfo.magic[ 3 ] = "I";
	bootInfo.magic[ 4 ] = "C";

	SystemTable->BootServices->ExitBootServices( ImageHandle, MapKey );

	KrnlStart( &bootInfo );

	return EFI_SUCCESS;
}
