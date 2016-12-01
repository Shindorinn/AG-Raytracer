#pragma once
#include "Primitive.h"
#include "Light.h"
#include "Camera.h"

class Scene
{
public:
	Primitive* primitives[4];
	Light* lights[1];
	Camera* camera;
	Scene();
};
