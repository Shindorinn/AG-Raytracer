#pragma once

namespace ocllab {

class Texture;
class Shader 
{
public:
	// constructor / destructor
	Shader( const char* vfile, const char* pfile );
	~Shader();
	// get / set
	unsigned int GetID() { return id; }
	// methods
	void Init( const char* vfile, const char* pfile );
	void Compile( const char* vtext, const char* ftext );
	void Bind();
	void SetInputTexture( unsigned int slot, const char* name, Texture* texture );
	void SetInputMatrix( const char* name, const mat4& matrix );
	void Unbind();
private:
	// data members
	unsigned int id;		// shader program identifier
	unsigned int vertex;	// vertex shader identifier
	unsigned int pixel;		// fragment shader identifier
};

}

// EOF