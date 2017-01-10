#include "system.h"

// textFileRead
// Store the coontents of a text file in a string variable.
// ----------------------------------------------------------------------------
static string textFileRead( const char* _File ) 
{
	string data, line;
	ifstream f( _File );
	if (f.is_open()) 
	{ 
		while (!f.eof()) 
		{
			getline( f, line );
			data.append( line );
			data.append( "\n" );
		}
		f.close();
	}
	return data;
}

// Shader constructor
// Create a shader based on vertex and fragment shader source files.
// ----------------------------------------------------------------------------
Shader::Shader( const char* vfile, const char* pfile )
{
    Init( vfile, pfile );
}

// Shader destructor
// ----------------------------------------------------------------------------
Shader::~Shader()
{
	glDetachShader( id, pixel );
	glDetachShader( id, vertex );
	glDeleteShader( pixel );
	glDeleteShader( vertex );
	glDeleteProgram( id );
}

// Init method
// Load source files and compile shader.
// ----------------------------------------------------------------------------
void Shader::Init( const char* vfile, const char* pfile ) 
{
	string vsText = textFileRead( vfile );
	string fsText = textFileRead( pfile );
	if (vsText.size() == 0) ERRORMESSAGE( vfile, "File not found" );
	if (fsText.size() == 0) ERRORMESSAGE( pfile, "File not found" );
	const char* vertexText = vsText.c_str();
	const char* fragmentText = fsText.c_str();
	Compile( vertexText, fragmentText );
}

// Compile method
// Shader compilation.
// ----------------------------------------------------------------------------
void Shader::Compile( const char* vtext, const char* ftext )
{
	vertex = glCreateShader( GL_VERTEX_SHADER );
	pixel = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( vertex, 1, &vtext, 0 );
	glCompileShader( vertex );
	CheckShader( vertex, vtext, ftext );
	glShaderSource( pixel, 1, &ftext, 0 );
	glCompileShader( pixel );
	CheckShader( pixel, vtext, ftext );
	id = glCreateProgram();
	glAttachShader( id, vertex );
	glAttachShader( id, pixel );
	glBindAttribLocation( id, 0, "in_Position" );
	glBindAttribLocation( id, 1, "in_Color" );
	glLinkProgram( id );
	CheckProgram( id, vtext, ftext );
}

// Bind method
// Bind the shader for rendering.
// ----------------------------------------------------------------------------
void Shader::Bind() 
{ 
    glUseProgram( id );
}

// SetInputTexture method
// Link a texture to the shader.
// ----------------------------------------------------------------------------
void Shader::SetInputTexture( unsigned int slot, const char* name, Texture* texture )
{
	glActiveTexture( slot );
	glBindTexture( GL_TEXTURE_2D, texture->GetID() );
	glUniform1i( glGetUniformLocation( id, name ), slot );
}

// SetInputMatrix
// Copy a matrix to a uniform variable in the shader.
// ----------------------------------------------------------------------------
void Shader::SetInputMatrix( const char* name, const mat4& matrix )
{
	const GLfloat* data = (const GLfloat*)&matrix;
	glUniformMatrix4fv( glGetUniformLocation( id, name ), 1, GL_FALSE, data );
}

// Unbind method
// ----------------------------------------------------------------------------
void Shader::Unbind() 
{
    glUseProgram( 0 );
}

// EOF