#pragma once
#include "Ray.h"
#include "template.h"
#include "Scene.h"

class Renderer
{
public:
	Surface* renderSurface;
	Scene* scene;
	Pixel buffer[SCRHEIGHT][SCRWIDTH];
	Renderer(Scene* scene, Surface* renderSurface);

	void Render();
	Pixel Trace(Ray* ray, int x, int y);
	float DirectIllumination(vec3 intersectionPoint, vec3 surfaceNormal, Light* lightSource);
};
