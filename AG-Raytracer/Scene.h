#pragma once
#include "Primitive.h"
#include "Light.h"
#include "Camera.h"

#define TUNNEL_SCENE 1
#define MIRROR_SCENE 0
#define OBJ_LOAD 0

class Scene
{
public:
#if TUNNEL_SCENE
	Primitive* primitives[8];
	Light* lights[2];
#elif MIRROR_SCENE
	Primitive* primitives[9];
	Light* lights[2];
#elif TINY_SCENE
	Primitive* primitives[1];
	Light* lights[2];
#elif OBJ_LOAD
	Primitive* primitives[12];
	Light* lights[1];
#endif

	Camera* camera;
	Scene();
};
