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

}

void Game::MouseUp(int _Button) { /* implement if you want to detect mouse button presses */ }
void Game::MouseDown(int _Button) { /* implement if you want to detect mouse button presses */ }
void Game::MouseMove(int _X, int _Y) { /* implement if you want to detect mouse movement */ }
void Game::KeyUp(int a_Key) { /* implement if you want to handle keys */ }
void Game::KeyDown(int a_Key) { 
	/* implement if you want to handle keys */
	// find other keys here: http://sdl.beuc.net/sdl.wiki/SDLKey

	switch (a_Key)
	{
	case SDLK_w: // Move forward according to current viewing direction

		break;
	case SDLK_s: // Move backward according to current viewing direction

		break;
	case SDLK_a: // Strafe left according to current viewing direction

		break;

	case SDLK_d: // Strafe right according to current viewing direction

		break;

	case SDLK_q: // Rotate left according to current viewing direction

		break;

	case SDLK_e: // Rotate right according to current viewing direction

		break;

	case SDLK_r: // Reset to starting position

		break;

	case SDLK_c: // Move down according to current viewing direction
				 // TODO : Maybe make this world aligned only?
		break;

	case SDLK_SPACE: // Move up according to current viewing direction
					 // TODO : Maybe make this world aligned only?
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