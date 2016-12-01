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
void Game::KeyUp(int a_Key) {
	/* implement if you want to handle keys */ 
	this->KeySwitch(a_Key, 0);
}
void Game::KeyDown(int a_Key) { 
	this->KeySwitch(a_Key, 1);
}

void Game::KeySwitch(int key, int changedBy) {
	/* implement if you want to handle keys */
	// find other keys here: http://sdl.beuc.net/sdl.wiki/SDLKey

	switch (key)
	{
	case SDLK_w: // Move forward according to current viewing direction
		moveForward = changedBy;
		break;
	case SDLK_s: // Move backward according to current viewing direction
		moveBackward = changedBy;
		break;
	case SDLK_a: // Strafe left according to current viewing direction
		strafeLeft = changedBy;
		break;

	case SDLK_d: // Strafe right according to current viewing direction
		strafeRight = changedBy;
		break;

	case SDLK_q: // Rotate left according to current viewing direction
		turnLeft = changedBy;
		break;

	case SDLK_e: // Rotate right according to current viewing direction
		turnRight = changedBy;
		break;

	case SDLK_r: // Reset to starting position
		resetPosition = changedBy;
		break;

	case SDLK_c: // Move down according to current viewing direction
		moveDown = changedBy; // TODO : Maybe make this world aligned only?
		break;

	case SDLK_SPACE: // Move up according to current viewing direction
		moveUp = changedBy;	 // TODO : Maybe make this world aligned only?
		break;

	case SDLK_PLUS: // Increase FoV by increasing d in Camera
		increaseFOV = changedBy;
		break;

	case SDLK_MINUS: // Decrease FoV by decreasing d in Camera
		decreaseFOV = changedBy;
		break;

	default:
		break;
	}
}

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