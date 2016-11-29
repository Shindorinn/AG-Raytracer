#pragma once
#include "Ray.h"
#include "template.h"
#include "Scene.h"

class Renderer
{
public:
	Scene* scene;
	Pixel buffer[SCRWIDTH][SCRHEIGHT];

	Pixel Trace(Ray* ray, int x, int y);
	Renderer(Scene* scene);
};
