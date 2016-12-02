#include "template.h"
#include "Scene.h"
#include "Renderer.h"
Renderer* renderer;
// -----------------------------------------------------------
// Initialize the game
// -----------------------------------------------------------
void Game::Init()
{
	Scene* myScene = new Scene();
	renderer = new Renderer(myScene, renderSurface);
}

// -----------------------------------------------------------
// Input handling
// -----------------------------------------------------------
void Game::HandleInput(float dt)
{
	Camera* camera = renderer->scene->camera;
	vec3 currentViewDirection = camera->viewDirection;
	
	// Adjust viewdirection
	vec3 newViewDirection = vec3(0, 0, 1); // TODO: ADJUST
	vec3 right = cross(newViewDirection, vec3(0, 1, 0));
	vec3 up = cross(right, newViewDirection);
	// Adjust translation based on new viewdirection

	// Replace transformMatrix for camera

	//moveForward;
	//moveBackward;
	//strafeRight;
	//strafeLeft;
	//moveUp;
	//moveDown;
	//turnRight;
	//turnLeft;
	//increaseFOV;
	//decreaseFOV;
	//resetPosition;
}

void Game::MouseUp(int _Button) { /* implement if you want to detect mouse button presses */ }
void Game::MouseDown(int _Button) { /* implement if you want to detect mouse button presses */ }
void Game::MouseMove(int _X, int _Y) { /* implement if you want to detect mouse movement */ }
void Game::KeyUp(int a_Key) {/* implement if you want to handle keys */}
void Game::KeyDown(int a_Key) {/* implement if you want to handle keys */}

// -----------------------------------------------------------
// Main game tick function
// -----------------------------------------------------------
void Game::Tick(float dt)
{
	//renderSurface->Clear(0);
	//renderSurface->Print("hello world", 2, 2, 0xffffff);
	//renderSurface->Line(2, 10, 50, 10, 0xff0000);
	//printf("New tick! Delay : %f", dt);
	renderer->Render();
}