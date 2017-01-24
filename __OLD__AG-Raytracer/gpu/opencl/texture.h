#pragma once

namespace ocllab {

class Texture
{
public:
	enum
	{
		DEFAULT = 0,
		FLOAT = 1
	};
	// constructor / destructor
	Texture( unsigned int width, unsigned int height, unsigned int type = DEFAULT );
	Texture( char* fileName );
	// get / set
	unsigned int GetID() { return id; }
	// methods
	void Bind();
private:
	// data members
	GLuint id;
};

}

// EOF