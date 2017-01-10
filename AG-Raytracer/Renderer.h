#pragma once

class Renderer
{
public:
	Surface* renderSurface;
	Scene* scene;
	Pixel buffer[SCRHEIGHT][SCRWIDTH];
	Renderer(Scene* scene, Surface* renderSurface);

	void Render();

	vec3 Trace(Ray* ray);
	vec3 Sample(Ray* ray, int depth);
	vec3 CosineWeightedDiffuseReflection(vec3 normal);

	vec3 DirectIllumination(vec3 intersectionPoint, vec3 direction, vec3 normal, Light* lightSource, Material material);

	vec3 Reflect(vec3 direction, vec3 normal);
};
