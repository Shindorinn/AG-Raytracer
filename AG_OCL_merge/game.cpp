#include "system.h"

Tmpl8::Renderer* renderer;

// -----------------------------------------------------------
// Initialize the game
// -----------------------------------------------------------
void Tmpl8::Game::Init()
{
	Tmpl8::Scene* myScene = new Scene();
	renderer = new Renderer(myScene, renderSurface);

#if OCL_GAME_TMPL == 1
	// load shader and texture
	clOutput = new Tmpl8::Texture(SCRWIDTH, SCRHEIGHT, Tmpl8::Texture::FLOAT);
	shader = new Tmpl8::Shader("shaders/vignette.vert", "shaders/vignette.frag");
	// load OpenCL code
	testFunction = new Tmpl8::Kernel("programs/program.cl", "TestFunction");
	// link cl output texture as an OpenCL buffer
	outputBuffer = new Tmpl8::Buffer(clOutput->GetID(), Tmpl8::Buffer::TARGET);
	testFunction->SetArgument(0, outputBuffer);
#endif
}

// -----------------------------------------------------------
// Input handling
// -----------------------------------------------------------
#define DEBUG 0

void Tmpl8::Game::HandleInput(float dt) {}
void Tmpl8::Game::MouseUp(int _Button) { /* implement if you want to detect mouse button presses */ }
void Tmpl8::Game::MouseDown(int _Button) { /* implement if you want to detect mouse button presses */ }
void Tmpl8::Game::MouseMove(int _X, int _Y) { /* implement if you want to detect mouse movement */ }
	 

void Tmpl8::Game::KeyUp(int a_Key) {}
void Tmpl8::Game::KeyDown(int a_Key)
{
	Tmpl8::Camera* camera = renderer->scene->camera;
	mat4 transform = camera->transformMatrix;
	mat4 previousTransform = transform;

	float rotationSpeed = 0.05f;
	float movementSpeed = 0.1f;
	// Adjust viewdirection

	//LeftRight
	if (a_Key == 79 || a_Key == 80)
	{
		if (a_Key == 80)
			//transform = rotate(transform, -1 * rotationSpeed, camera->rUp);
			printf("left");
		else
			printf("");
			//transform = rotate(transform, rotationSpeed, camera->rUp);
		camera->TransformCamera(transform);
		camera->transformMatrix = previousTransform;

	}
	//UpDown
	if (a_Key == 82 || a_Key == 81)
	{
		if (a_Key == 82)
			//transform = rotate(transform, -1 * rotationSpeed, camera->rRight);
			printf("");
		else
			//transform = rotate(transform, rotationSpeed, camera->rRight);
			printf("");
		camera->TransformCamera(transform);
		camera->transformMatrix = previousTransform;

	}
	//Left/RightCtrl
	if (a_Key == 224 || a_Key == 228)
	{
		if (a_Key == 224)
			//transform = rotate(transform, -1 * rotationSpeed, camera->viewDirection);
			printf("");
		else
			//transform = rotate(transform, rotationSpeed, camera->viewDirection);
			printf("");
		camera->TransformCamera(transform);
		camera->transformMatrix = previousTransform;

	}
	//a,d
	if (a_Key == 4 || a_Key == 7)
	{
		if (a_Key == 4)
			//transform = translate(transform, -camera->rRight *movementSpeed);
			printf("");

		else
			//transform = translate(transform, camera->rRight *movementSpeed);
			printf("");
		camera->TransformCamera(transform);
		camera->transformMatrix = previousTransform;
	}

	//w,s
	if (a_Key == 26 || a_Key == 22)
	{
		if (a_Key == 26)
			//transform = translate(transform, camera->viewDirection *movementSpeed);
			printf("");

		else
			//transform = translate(transform, -camera->viewDirection * movementSpeed);
			printf("");
		camera->TransformCamera(transform);
		camera->transformMatrix = previousTransform;
	}

	//lShift-rShift
	if (a_Key == 225 || a_Key == 229)
	{
		if (a_Key == 225)
			//transform = translate(transform, camera->rUp *movementSpeed);
			printf("");

		else
			//transform = translate(transform, -camera->rUp  *movementSpeed);
			printf("");
		camera->TransformCamera(transform);
		camera->transformMatrix = previousTransform;
	}

}

// -----------------------------------------------------------
// Main game tick function
// -----------------------------------------------------------
void Tmpl8::Game::Tick(float dt)
{
#if OCL_GAME_TMPL == 1
	testFunction->Run(outputBuffer);
	shader->Bind();
	shader->SetInputTexture(GL_TEXTURE0, "color", clOutput);
	shader->SetInputMatrix("view", mat4::identity());
	DrawQuad();
#else
	renderer->Render();
	char buffer[500];
	sprintf(
		buffer,
		"FPS: %f \n Resolution : %i x %i ",
		1 / dt,
		SCRWIDTH,
		SCRHEIGHT);
	renderSurface->Print(buffer, 2, 2, 0xffffff);
#endif

}