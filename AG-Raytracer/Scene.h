#pragma once
#include "Primitive.h"
#include "Light.h"
#include "Camera.h"

#define TUNNEL_SCENE 0
#define MIRROR_SCENE 1
#define TINY_SCENE 1

class Scene
{
public:
<<<<<<< Updated upstream
#if TUNNEL_SCENE
	Primitive* primitives[8];
	Light* lights[2];
#elif MIRROR_SCENE
	Primitive* primitives[9];
	Light* lights[2];
#elif TINY_SCENE
	Primitive* primitives[1];
	Light* lights[2];
#endif
=======
	Primitive* primitives[12];
	Light* lights[1];
>>>>>>> Stashed changes
	Camera* camera;
	Scene();
};
