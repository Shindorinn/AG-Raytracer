#include "system.h"

// Texture constructor
// Creates a new OpenGL texture. Two types are available:
// DEFAULT: 32-bit integer ARGB texture;
// OPENCL:  128-bit floating point ARGB texture to be used with OpenCL.
// ----------------------------------------------------------------------------
Texture::Texture( unsigned int width, unsigned int height, unsigned int type )
{
	glGenTextures( 1, &id );
	glBindTexture( GL_TEXTURE_2D, id );
	if (type == DEFAULT)
	{
		// regular texture
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, 0 );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	}
	else /* type == FLOAT */
	{
		// texture to be used with OpenCL code
		float* data = new float[width * height * 4];
		memset( data, 0, width * height * 16 );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGB, GL_FLOAT, data );
	}
	CheckGL();
}

// Texture constructor
// Loads an image using FreeImage and creates an OpenGL texture for it.
// ----------------------------------------------------------------------------
Texture::Texture( char* fileName )
{
	GLuint textureType = GL_TEXTURE_2D;
	glGenTextures( 1, &id );
	glBindTexture( textureType, id );
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType( fileName, 0 );
	if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename( fileName );
	FIBITMAP* tmp = FreeImage_Load( fif, fileName );
	if (!tmp) ERRORMESSAGE( fileName, "File not found" );
	FIBITMAP* dib = FreeImage_ConvertTo24Bits( tmp );
	FreeImage_Unload( tmp );
	unsigned int width = FreeImage_GetWidth( dib );
	unsigned int height = FreeImage_GetHeight( dib );
	unsigned int* data = new unsigned int[width * height];
	unsigned char* line = new unsigned char[width * 3];
	for( unsigned int y = 0; y < height; y++) 
	{
		memcpy( line, FreeImage_GetScanLine( dib, height - 1 - y ), width * 3 );
		for( unsigned int x = 0; x < width; x++ ) data[y * width + x] = (line[x * 3 + 2] << 16) + (line[x * 3 + 1] << 8) + line[x * 3 + 0];
	}
	FreeImage_Unload( dib );
	glTexImage2D( textureType, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data );
	delete data;
	glTexParameteri( textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glGenerateMipmap( textureType );
}

// Bind method
// Binds the texture for rendering.
// ----------------------------------------------------------------------------
void Texture::Bind()
{
	glBindTexture( GL_TEXTURE_2D, id );
}

// EOF