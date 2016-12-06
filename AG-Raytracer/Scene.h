#pragma once
#include "Primitive.h"
#include "Light.h"
#include "Camera.h"

class Scene
{
public:
	Primitive* primitives[8];
	Light* lights[2];
	Camera* camera;
	Scene();
};
