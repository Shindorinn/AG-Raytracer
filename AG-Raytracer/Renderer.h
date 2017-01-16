#pragma once

class Renderer
{
public:
	Surface* renderSurface;
	Scene* scene;
	Pixel buffer[SCRHEIGHT][SCRWIDTH];
	Renderer(Scene* scene, Surface* renderSurface);

	vec3 accumulator[SCRHEIGHT][SCRWIDTH];
	int frameCount;

	int Render();

	vec3 Trace(Ray* ray);
	vec3 Sample(Ray* ray, int depth, bool secondaryRay = false);
	vec3 BasicSample(Ray* ray, int depth);
	vec3 CosineWeightedDiffuseReflection(vec3 normal);


	vec3 DirectSampleLights(vec3 intersect, vec3 normal);

	vec3 DirectIllumination(vec3 intersectionPoint, vec3 direction, vec3 normal, Light* lightSource, Material material);

	vec3 Reflect(vec3 direction, vec3 normal);
};
