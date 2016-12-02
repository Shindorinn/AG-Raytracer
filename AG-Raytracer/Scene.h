﻿#pragma once
#include "Primitive.h"
#include "Light.h"
#include "Camera.h"

class Scene
{
public:
	Primitive* primitives[2];
	Light* lights[2];
	Camera* camera;
	Scene();
};
