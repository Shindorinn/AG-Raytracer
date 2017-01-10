#include "system.h"

static HGLRC hrc;
static HDC hdc;

// CheckGL
// ----------------------------------------------------------------------------
void CheckGL()
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		char t[1024];
		sprintf( t, "error %i (%x)\n", error, error );
		if (error == 0x500) strcat( t, "INVALID ENUM" );
		else if (error == 0x502) strcat( t, "INVALID OPERATION" );
		else if (error == 0x501) strcat( t, "INVALID VALUE" );
		else if (error == 0x506) strcat( t, "INVALID FRAMEBUFFER OPERATION" );
		else strcat( t, "UNKNOWN ERROR" );
		FATALERROR( "OpenGL error" );
	}
}

// CreateGLContext
// ----------------------------------------------------------------------------
bool CreateGLContext()
{
	PIXELFORMATDESCRIPTOR pfd;
	hdc = GetDC( GetWindowHandle() );
	memset( &pfd, 0, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_SUPPORT_OPENGL|PFD_DRAW_TO_WINDOW|PFD_DOUBLEBUFFER_DONTCARE|PFD_STEREO_DONTCARE|PFD_DEPTH_DONTCARE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int nPixelFormat = ChoosePixelFormat( hdc, &pfd );
	if (nPixelFormat == 0) return false;
	int bResult = SetPixelFormat( hdc, nPixelFormat, &pfd );
	if (!bResult) return false;
#ifdef USE_GLEE
	// unsafe but fast path using Glee
	hrc = hrc = wglCreateContext( hdc );
	wglMakeCurrent( hdc, hrc );
	printf( "OpenGL device: %s\n", glGetString( GL_RENDERER ) );
	GLeeMinimalInit();
#else
	// safe but slow path using Glew
	HGLRC tempOpenGLContext = wglCreateContext( hdc );
	wglMakeCurrent( hdc, tempOpenGLContext );
	glewExperimental = TRUE;
	GLenum error = glewInit();
	if (error != GLEW_OK) return false;
	int attributes[] = 
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3, WGL_CONTEXT_MINOR_VERSION_ARB, 2,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, 0
	};
	if (wglewIsSupported( "WGL_ARB_create_context" ) == 1) 
	{
		hrc = wglCreateContextAttribsARB( hdc, NULL, attributes );
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( tempOpenGLContext );	// delete temporary OpenGL 2.1 context
		wglMakeCurrent( hdc, hrc );				// make 3.0 context current
	}
	else hrc = tempOpenGLContext;				// no support for OpenGL 3.x and up, use 2.1
	printf( "OpenGL device: %s\n", glGetString( GL_RENDERER ) );
#endif
	glViewport( 0, 0, SCRWIDTH, SCRHEIGHT );
	glDisable( GL_BLEND );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );
	return true;
}

// Present
// ----------------------------------------------------------------------------
void Present()
{
	SwapBuffers( hdc );
}

// ShutdownGL
// ----------------------------------------------------------------------------
void ShutdownGL()
{
	wglMakeCurrent( hdc, 0 );
    wglDeleteContext( hrc );
    ReleaseDC( GetWindowHandle(), hdc );
}

// CreateVBO
// ----------------------------------------------------------------------------
GLuint CreateVBO( const GLfloat* data, const unsigned int size )
{
	GLuint id;
	glGenBuffers( 1, &id );
	glBindBuffer( GL_ARRAY_BUFFER, id );
	glBufferData( GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW );
	return id;
}

// BindVBO
// ----------------------------------------------------------------------------
void BindVBO( const unsigned int idx, const unsigned int N, const GLuint id )
{
	glEnableVertexAttribArray( idx );
	glBindBuffer( GL_ARRAY_BUFFER, id );
	glVertexAttribPointer( idx, N, GL_FLOAT, GL_FALSE, 0, (void*)0 );
}

// CheckFrameBuffer
// ----------------------------------------------------------------------------
void CheckFrameBuffer()
{
	if (glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE) return;
	FATALERROR( "Incomplete frame buffer" );
}

// CheckShader
// ----------------------------------------------------------------------------
void CheckShader( GLuint shader, const char* vshader, const char* fshader ) 
{
	char buffer[1024];
	memset( buffer, 0, 1024 );
	GLsizei length = 0;
	glGetShaderInfoLog( shader, 1024, &length, buffer );
	if (length > 0) if (strstr( buffer, "ERROR" )) ERRORMESSAGE( buffer, "Shader compile error" );
}

// CheckProgram
// ----------------------------------------------------------------------------
void CheckProgram( GLuint id, const char* vshader, const char* fshader ) 
{
	char buffer[1024];
	memset( buffer, 0, 1024 );
	GLsizei length = 0;
	glGetProgramInfoLog( id, 1024, &length, buffer );
	if (length > 0) if (!strstr( buffer, "No errors" )) ERRORMESSAGE( buffer, "Shader link error" );
}

// DrawQuad
// ----------------------------------------------------------------------------
void DrawQuad()
{
	static GLuint vao = 0;
	if (!vao)
	{
		// generate buffers
		GLfloat verts[] = { -1, -1, 0, 1, -1, 0, -1, 1, 0, 1, -1, 0, -1, 1, 0, 1, 1, 0 };
		GLfloat uvdata[] = { 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0 };
		GLuint vertexBuffer = CreateVBO( verts, sizeof( verts ) );
		GLuint UVBuffer = CreateVBO( uvdata, sizeof( uvdata ) );
		glGenVertexArrays( 1, &vao );
		glBindVertexArray( vao );
		BindVBO( 0, 3, vertexBuffer );
		BindVBO( 1, 2, UVBuffer );
		glBindVertexArray( 0 );
		glDisableVertexAttribArray( 0 );
		glDisableVertexAttribArray( 1 );
	}
	glBindVertexArray( vao );
	glDrawArrays( GL_TRIANGLES, 0, 6 );
	glBindVertexArray( 0 );
}

// EOF