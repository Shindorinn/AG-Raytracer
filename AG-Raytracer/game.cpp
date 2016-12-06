#include "template.h"
#include "Scene.h"
#include "Renderer.h"


Renderer* renderer;

float printout_d				= 0.0f;
float printout_position_x		= 0.0f;
float printout_position_y		= 0.0f;
float printout_position_z		= 0.0f;
float printout_viewDirection_x  = 0.0f;
float printout_viewDirection_y  = 0.0f;
float printout_viewDirection_z  = 0.0f;
// -----------------------------------------------------------
// Initialize the game
// -----------------------------------------------------------
void Game::Init()
{
	Scene* myScene = new Scene();
	renderer = new Renderer(myScene, renderSurface);

	printout_d = renderer->scene->camera->d;
	printout_position_x = renderer->scene->camera->position.x;
	printout_position_y = renderer->scene->camera->position.y;
	printout_position_z = renderer->scene->camera->position.z;
	printout_viewDirection_x = renderer->scene->camera->viewDirection.x;
	printout_viewDirection_y = renderer->scene->camera->viewDirection.y;
	printout_viewDirection_z = renderer->scene->camera->viewDirection.z;
}

// -----------------------------------------------------------
// Input handling
// -----------------------------------------------------------
#define DEBUG 0

void Game::HandleInput(float dt){}
void Game::MouseUp(int _Button) { /* implement if you want to detect mouse button presses */ }
void Game::MouseDown(int _Button) { /* implement if you want to detect mouse button presses */ }
void Game::MouseMove(int _X, int _Y) { /* implement if you want to detect mouse movement */ }


void Game::KeyUp(int a_Key) {}
void Game::KeyDown(int a_Key)
{
	Camera* camera = renderer->scene->camera;
	mat4 transform = camera->transformMatrix;
	mat4 previousTransform = transform;

	float rotationSpeed = 0.05f;
	float movementSpeed = 0.1f;
	// Adjust viewdirection

	//LeftRight
	if (a_Key == 79 || a_Key == 80)
	{
		if (a_Key == 80)
			transform = rotate(transform, -1 * rotationSpeed, camera->rUp);
		else
			transform = rotate(transform, rotationSpeed, camera->rUp);
		camera->TransformCamera(transform);
		camera->transformMatrix = previousTransform;

	}
	//UpDown
	if (a_Key == 82 || a_Key == 81)
	{
		if (a_Key == 82)
			transform = rotate(transform, -1 * rotationSpeed, camera->rRight);
		else
			transform = rotate(transform, rotationSpeed, camera->rRight);
		camera->TransformCamera(transform);
		camera->transformMatrix = previousTransform;

	}
	//Left/RightCtrl
	if (a_Key == 224 || a_Key == 228)
	{
		if (a_Key == 224)
			transform = rotate(transform, -1 * rotationSpeed, camera->viewDirection);
		else
			transform = rotate(transform, rotationSpeed, camera->viewDirection);
		camera->TransformCamera(transform);
		camera->transformMatrix = previousTransform;

	}
	//a,d
	if (a_Key == 4 || a_Key == 7)
	{
		if (a_Key == 4)
			transform = translate(transform, -camera->rRight *movementSpeed);

		else
			transform = translate(transform, camera->rRight *movementSpeed);
		camera->TransformCamera(transform);
		camera->transformMatrix = previousTransform;
	}

	//w,s
	if (a_Key == 26 || a_Key == 22)
	{
		if (a_Key == 26)
			transform = translate(transform, camera->viewDirection *movementSpeed);

		else
			transform = translate(transform, -camera->viewDirection * movementSpeed);
		camera->TransformCamera(transform);
		camera->transformMatrix = previousTransform;
	}

	//lShift-rShift
	if (a_Key == 225 || a_Key == 229)
	{
		if (a_Key == 225)
			transform = translate(transform, camera->rUp *movementSpeed);

		else
			transform = translate(transform, -camera->rUp  *movementSpeed);
		camera->TransformCamera(transform);
		camera->transformMatrix = previousTransform;
	}


	printf("%i", a_Key);
}

// -----------------------------------------------------------
// Main game tick function
// -----------------------------------------------------------
void Game::Tick(float dt)
{
	renderer->Render();

	char buffer[500];
	sprintf(
		buffer,
		"FPS: %f \n Resolution : %f x %f \n d : %f \n Camera Position : %f, %f, %f \n View Direction : %f, %f, %f",
		1 / dt,
		SCRWIDTH,
		SCRHEIGHT,
		printout_d,
		printout_position_x,
		printout_position_y,
		printout_position_z,
		printout_viewDirection_x,
		printout_viewDirection_y,
		printout_viewDirection_z
		);
	renderSurface->Print(buffer, 2, 2, 0xffffff);
}