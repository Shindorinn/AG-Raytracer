#include "system.h"

static Texture* clOutput = 0;
static Shader* shader = 0;
static Kernel* testFunction;
static Buffer* outputBuffer;

bool Game::Init()
{
	// load shader and texture
	clOutput = new Texture( SCRWIDTH, SCRHEIGHT, Texture::FLOAT );
	shader = new Shader( "shaders/vignette.vert", "shaders/vignette.frag" );
	// load OpenCL code
	testFunction = new Kernel( "programs/program.cl", "TestFunction" );
	// link cl output texture as an OpenCL buffer
	outputBuffer = new Buffer( clOutput->GetID(), Buffer::TARGET );
	testFunction->SetArgument( 0, outputBuffer );
	// done
	return true;
}

void Game::Tick()
{
	testFunction->Run( outputBuffer );
	shader->Bind();
	shader->SetInputTexture( GL_TEXTURE0, "color", clOutput );
	shader->SetInputMatrix( "view", mat4::Identity() );
	DrawQuad();
}

void Game::Shutdown()
{
}

// EOF