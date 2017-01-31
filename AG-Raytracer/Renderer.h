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
	int numberOfLights;

	int pixelNumber;

	int Render();

	vec3 Trace(Ray* ray, bool isShadowRay = false);

	vec3 Sample(Ray* ray, int depth, bool secondaryRay = false);
	vec3 BasicSample(Ray* ray, int depth);
	vec3 SampleMIS(Ray* ray);

	vec3 CosineWeightedDiffuseReflection(vec3 normal);

	vec3 DirectSampleLights(vec3 intersect, vec3 normal, Material material, bool isMis = false);

	vec3 Refract(bool inside, vec3 D, vec3 N);
	vec3 DiffuseReflection(vec3 normal);

	glm::uint RandomInt(glm::uint * seed);

	float RandomFloat(glm::uint * seed);

	glm::uint TauStep(int s1, int s2, int s3, uint M, glm::uint* seed);
	glm::uint HQIRand(glm::uint* seed);
	glm::uint SeedRandom(glm::uint s);
};
