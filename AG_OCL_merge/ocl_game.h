#pragma once

namespace ocllab {

class Game
{
public:
	bool Init();
	void Tick();
	void Shutdown();
	void KeyDown( unsigned int key ) {}
	void KeyUp( unsigned int key ) {}
	void MouseMove( int x, int y ) {}
	void MouseClick() {}
};

}

// EOF