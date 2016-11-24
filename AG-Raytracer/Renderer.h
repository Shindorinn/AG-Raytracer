#pragma once
#include "Ray.h"
#include "template.h"
#include "Scene.h"

class Renderer
{
public:
	Scene* scene;
	vec3 Trace(Ray* ray);
	Renderer(Scene* scene);
};
