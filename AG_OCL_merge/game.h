#pragma once

#define OCL_GAME_TMPL 1

//namespace Tmpl8 {

class Surface;
class Game
{
public:
	void SetTarget( Surface* _Surface ) { renderSurface = _Surface; }
	void Init();
	void Shutdown() { /* implement if you want code to be executed upon app exit */ };
	void HandleInput( float dt );
	void Tick( float dt );
	void MouseUp(int _Button);
	void MouseDown(int _Button);
	void MouseMove(int _X, int _Y);
	void KeyUp(int a_Key);
	void KeyDown(int a_Key);
private:
	Surface* renderSurface;
#if OCL_GAME_TMPL == 1
	Tmpl8::Texture* clOutput = 0;
	Tmpl8::Shader* shader = 0;
	Tmpl8::Kernel* GPURenderFunction;
	Tmpl8::Buffer* outputBuffer;
#endif

	int moveForward;
	int moveBackward;
	int strafeRight;
	int strafeLeft;
	int moveUp;
	int moveDown;
	int turnRight;
	int turnLeft;
	int increaseFOV;
	int decreaseFOV;
	int resetPosition;
};

//}; // namespace Tmpl8