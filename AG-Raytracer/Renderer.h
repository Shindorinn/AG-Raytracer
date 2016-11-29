#pragma once
#include "Ray.h"
#include "template.h"
#include "Scene.h"

class Renderer
{
public:
	Scene* scene;
	Pixel buffer[SCRWIDTH][SCRHEIGHT];
	Renderer(Scene* scene);

	Pixel Trace(Ray* ray, int x, int y);
	float DirectIllumination(vec3 intersectionPoint, vec3 surfaceNormal);
};
