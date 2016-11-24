#include "template.h"
#include "Scene.h"
#include "Renderer.h"

// -----------------------------------------------------------
// Initialize the game
// -----------------------------------------------------------
void Game::Init()
{
	Scene* myScene = new Scene();
	Renderer* renderer = new Renderer(myScene);
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
	screen->Clear(0);
	screen->Print("hello world", 2, 2, 0xffffff);
	screen->Line(2, 10, 50, 10, 0xff0000);
}