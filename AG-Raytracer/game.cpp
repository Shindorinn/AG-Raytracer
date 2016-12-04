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
#define VK_W 0x57
#define VK_S 0x53
#define VK_A 0x41
#define VK_D 0x44
#define VK_C 0x43

#define VK_MASK_PRESSED_DOWN 1 << 16

void Game::HandleInput(float dt)
{
	bool w		= GetAsyncKeyState(VK_W) && VK_MASK_PRESSED_DOWN;
	bool a		= GetAsyncKeyState(VK_A) && VK_MASK_PRESSED_DOWN;
	bool s		= GetAsyncKeyState(VK_S) && VK_MASK_PRESSED_DOWN;
	bool d		= GetAsyncKeyState(VK_D) && VK_MASK_PRESSED_DOWN;
	bool c		= GetAsyncKeyState(VK_C) && VK_MASK_PRESSED_DOWN;
	bool up		= GetAsyncKeyState(VK_UP) && VK_MASK_PRESSED_DOWN;
	bool down	= GetAsyncKeyState(VK_DOWN) && VK_MASK_PRESSED_DOWN;
	bool left	= GetAsyncKeyState(VK_LEFT) && VK_MASK_PRESSED_DOWN;
	bool right	= GetAsyncKeyState(VK_RIGHT) && VK_MASK_PRESSED_DOWN;
	bool rctrl	= GetAsyncKeyState(VK_RCONTROL) && VK_MASK_PRESSED_DOWN;
	bool lctrl	= GetAsyncKeyState(VK_LCONTROL) && VK_MASK_PRESSED_DOWN;
	bool space = GetAsyncKeyState(VK_SPACE) && VK_MASK_PRESSED_DOWN;

#if DEBUG
	printf("Handling input! \n");
	printf(
		"Keystates! \n W	: %i \n A : %i \n S : %i \n D : %i \n C : %i \n UP : %i \n DOWN : %i \n LEFT : %i \n RIGHT : %i \n RCONTROL : %i \n LCONTROL : %i \n SPACEBAR : %i \n",
		GetAsyncKeyState(VK_W),
		GetAsyncKeyState(VK_A),
		GetAsyncKeyState(VK_S),
		GetAsyncKeyState(VK_D), 
		GetAsyncKeyState(VK_C), 
		GetAsyncKeyState(VK_UP), 
		GetAsyncKeyState(VK_DOWN), 
		GetAsyncKeyState(VK_LEFT),
		GetAsyncKeyState(VK_RIGHT), 
		GetAsyncKeyState(VK_RCONTROL), 
		GetAsyncKeyState(VK_LCONTROL), 
		GetAsyncKeyState(VK_SPACE));

	printf(
		"Bool States! \n W	: %i \n A : %i \n S : %i \n D : %i \n C : %i \n UP : %i \n DOWN : %i \n LEFT : %i \n RIGHT : %i \n RCONTROL : %i \n LCONTROL : %i \n SPACEBAR : %i \n",
		w		,
		a		,
		s		,
		d		,
		c		,
		space	,
		rctrl	,
		lctrl	,
		up		,
		down	,
		left	,
		right
		);
#endif
	Camera* camera = renderer->scene->camera;
	mat4 transform = camera->transformMatrix;

	float rotationSpeed = 1.0f;
	float movementSpeed = 10.0f;
	
	float rotationAmount = rotationSpeed * dt;

	// Adjust viewdirection
	transform = rotate(transform, rotationAmount * up + rotationAmount * -down, vec3(1, 0, 0));
	transform = rotate(transform, rotationAmount * left + rotationAmount -right, vec3(0, 1, 0));
	transform = rotate(transform, rotationAmount * rctrl + rotationAmount -lctrl, vec3(0, 0, 1));

	camera->TransformCamera(transform);

	// Adjust translation based on new viewdirection
	vec3 viewDirection = camera->viewDirection;
	vec3 rightDirection = cross(viewDirection, vec3(0, 1, 0));
	vec3 position = camera->position;
	
	vec3 movementMask = vec3(
		(a + -d),
		(c + -space),
		(w + -s)
	);
	
	float movementAmount = movementSpeed * dt;

	position += rightDirection	*	movementMask.x  * movementAmount;
	position.y	+=					movementMask.y	* movementAmount;
	position += viewDirection	*	movementMask.z	* movementAmount;

	transform = translate(transform, position);
	camera->TransformCamera(transform);
}

void Game::MouseUp(int _Button) { /* implement if you want to detect mouse button presses */ }
void Game::MouseDown(int _Button) { /* implement if you want to detect mouse button presses */ }
void Game::MouseMove(int _X, int _Y) { /* implement if you want to detect mouse movement */ }
void Game::KeyUp(int a_Key) {/* implement if you want to handle keys */ }
void Game::KeyDown(int a_Key) {/* implement if you want to handle keys */ }

// -----------------------------------------------------------
// Main game tick function
// -----------------------------------------------------------
void Game::Tick(float dt)
{
	//renderSurface->Clear(0);
	HandleInput(dt);
	renderer->Render();

	char buffer[100];
	sprintf(buffer, "FPS: %f", 1 / dt);
	renderSurface->Print(buffer, 2, 2, 0xffffff);
}