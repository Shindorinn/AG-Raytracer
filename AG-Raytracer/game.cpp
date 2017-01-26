#include "template.h"

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
#define DEBUG 0

void Game::HandleInput(float dt) {}
void Game::MouseUp(int _Button) { /* implement if you want to detect mouse button presses */ }
void Game::MouseDown(int _Button) { /* implement if you want to detect mouse button presses */ }
void Game::MouseMove(int _X, int _Y) { /* implement if you want to detect mouse movement */ }


void Game::KeyUp(int a_Key) {}
void Game::KeyDown(int a_Key)
{	
}

// -----------------------------------------------------------
// Main game tick function
// -----------------------------------------------------------
void Game::Tick(float dt)
{
	int pixelCount = renderer->Render();

	char buffer[500];
	sprintf(
		buffer,
		"FPS: %f \n Resolution : %i x %i ",
		1 / dt,
		SCRWIDTH,
		SCRHEIGHT);
	renderSurface->Print(buffer, 2, 2, 0xffffff);

	char pixCountBuffer[500];
	sprintf(
		pixCountBuffer, "Pixels summed: %i%", pixelCount);

	renderSurface->Print(pixCountBuffer, 300, 2, 0xffffff);
}