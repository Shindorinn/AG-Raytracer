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
	renderer = new Renderer(myScene);
}

// -----------------------------------------------------------
// Input handling
// -----------------------------------------------------------
void Game::HandleInput(float dt)
{
}

// -----------------------------------------------------------
// Main game tick function
// -----------------------------------------------------------
void Game::Tick(float dt)
{
	renderSurface->Clear(0);
	renderSurface->Print("hello world", 2, 2, 0xffffff);
	renderSurface->Line(2, 10, 50, 10, 0xff0000);

	for (int y = 0; y < SCRHEIGHT; y++)
		for (int x = 0; x < SCRWIDTH; x++)
			renderSurface->Plot(x, y, renderer->buffer[x][y]);

}