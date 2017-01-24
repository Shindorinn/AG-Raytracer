#pragma once

void CheckGL();
bool CreateGLContext();
void ShutdownGL();
GLuint CreateVBO( const GLfloat* data, const unsigned int size );
void BindVBO( const unsigned int idx, const unsigned int N, const GLuint id );
void CheckFrameBuffer();
void CheckShader( GLuint shader, const char* vshader, const char* fshader );
void CheckProgram( GLuint id, const char* vshader, const char* fshader );
void DrawQuad();
void Present();

// EOF