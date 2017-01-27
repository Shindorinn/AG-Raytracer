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

	vec3 DirectSampleLights(vec3 intersect, vec3 normal, Material material);

	vec3 Reflect(vec3 direction, vec3 normal);

	glm::uint RandomInt(glm::uint * seed);

	float RandomFloat(glm::uint * seed);

};
