#include "template.h"

#define DEBUG 1
#define EPSILON 0.01f

#define USEBVH 0

#define MAXRAYDEPTH 5

Renderer::Renderer(Scene* scene, Surface* renderSurface)
{
	this->scene = scene;
	this->renderSurface = renderSurface;
	this->scene->camera->GenerateRays();
	this->frameCount = 0;
}

//Int, so it can return the total pixel values summed
int Renderer::Render() {

	int pixelCount = 0;

	//Increase frameCount, which is used for averaging multiple frames of path tracing.
	frameCount++;

#pragma omp parallel for
	for (int y = 0; y < SCRHEIGHT; y++) {
		printf("Current y:%i% \n", y);
#pragma omp parallel for
		for (int x = 0; x < SCRWIDTH; x++)
		{
			vec3 colorResult = Sample(this->scene->camera->primaryRays[y*SCRWIDTH + x], 0);

			// First convert range
			colorResult *= 256.0f;
			//printf("r: %f%, g: %f%, b: %f% \n", colorResult.r, colorResult.g, colorResult.b);

			// Then clamp
			int r = min((int)colorResult.x, 255);
			int g = min((int)colorResult.y, 255);
			int b = min((int)colorResult.z, 255);

			// Retrieve current buffer values.
			int currentR = buffer[y][x] >> 16;
			int currentG = (buffer[y][x] >> 8) - (currentR << 8);
			int currentB = buffer[y][x] - (currentR << 16) - (currentG << 8);

			int nextR = currentR + (r - currentR) / static_cast<float>(frameCount);
			int nextG = currentG + (g - currentG) / static_cast<float>(frameCount);
			int nextB = currentB + (b - currentB) / static_cast<float>(frameCount);
			pixelCount += nextR + nextG + nextB;

			// Then merge

			buffer[y][x] = (nextR << 16) + (nextG << 8) + (nextB);
		}
	}
	//#pragma omp parallel for
	for (int y = 0; y < SCRHEIGHT; y++)
		//#pragma omp parallel for
		for (int x = 0; x < SCRWIDTH; x++)
			this->renderSurface->Plot(x, y, this->buffer[y][x]);

	return pixelCount;
}

vec3 Renderer::Sample(Ray* ray, int depth)
{
	if (depth > MAXRAYDEPTH)
	{
		return vec3(0);
	}
	// trace ray
	vec3 intersect = Trace(ray);
	// terminate if ray left the scene
	if (ray->t == INFINITY)
	{
		return vec3(0, 0, 0);
	}

	Entity* hit = ray->hit;

	// terminate if we hit a light source
	if (hit->isLight)
		return static_cast<Light*>(hit)->color;

	Primitive* primitiveHit = static_cast<Primitive*>(hit);

	vec3 normal = primitiveHit->GetNormal(intersect);

	// continue in random direction
	vec3 R = CosineWeightedDiffuseReflection(normal);
	Ray newRay = Ray(intersect, R);

	// update throughput

	//float BRDF = primitiveHit->material.albedo / PI;
	vec3 BRDF = vec3(primitiveHit->material.color.r / PI, primitiveHit->material.color.g / PI, primitiveHit->material.color.b / PI);

	vec3 Ei = Sample(&newRay, depth + 1) * dot(normal, R); // irradiance

	return PI * 2.0f * BRDF * Ei;
}

vec3 Renderer::CosineWeightedDiffuseReflection(vec3 normal)
{
	float r0 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX), r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float r = sqrt(r0);
	float theta = 2 * PI * r1;
	float x = r * cosf(theta);
	float y = r * sinf(theta);

	vec3 dir = vec3(x, y, sqrt(1 - r0));

	vec3 randomDir = normalize(vec3(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));

	vec3 t = cross(randomDir, normal);
	vec3 b = cross(normal, t);

	mat3 tangentSpace = mat3(b, t, normal);

	//TODO: CHECK THIS
	vec3 transformedDir = tangentSpace * dir;

	return transformedDir;
}

////Internet, https://www.shadertoy.com/view/4tl3z4
//vec3 cosWeightedRandomHemisphereDirection(const vec3 n) {
//	vec2 r = hash2();
//
//	vec3  uu = normalize(cross(n, vec3(0.0, 1.0, 1.0)));
//	vec3  vv = cross(uu, n);
//
//	float ra = sqrt(r.y);
//	float rx = ra*cos(6.2831*r.x);
//	float ry = ra*sin(6.2831*r.x);
//	float rz = sqrt(1.0 - r.y);
//	vec3  rr = vec3(rx*uu + ry*vv + rz*n);
//
//	return normalize(rr);
//}

vec3 Renderer::Trace(Ray* ray)
{
	float smallestT = INFINITY;

#if !USEBVH
	for (int x = 0; x < sizeof(this->scene->entities) / sizeof(this->scene->entities[0]); x++)
	{
		if (this->scene->entities[x]->CheckIntersection(ray) && smallestT> ray->t)
		{
			smallestT = ray->t;
			ray->hit = this->scene->entities[x];
		}
	}
	//for (int x = 0; x < sizeof(this->scene->lights) / sizeof(this->scene->lights[0]); x++)
	//{
	//	//TODO: save this AABB beforehand, or better, add it beforehand to the primitives.
	//	if (this->scene->lights[x]->sphere.CheckIntersection(ray) && smallestT> ray->t)
	//	{
	//		smallestT = ray->t;
	//		ray->hit = this->scene->lights[x];
	//	}

	//}


#else 
	scene->bvh->Traverse(ray, scene->bvh->rootNode);

	smallestT = ray->t;
#endif

	if (smallestT == INFINITY)
	{
		return vec3(0);
	}
	else
	{

		vec3 intersectionPoint = ray->origin + smallestT*ray->direction;
		//vec3 normal = hit->GetNormal(intersectionPoint);
		vec3 colorResult = vec3(0, 0, 0);

		return intersectionPoint;
	}
}

vec3 Renderer::DirectIllumination(vec3 intersectionPoint, vec3 direction, vec3 normal, Light* lightSource, Material material)
{
	vec3 intersectionWithEpsilon = intersectionPoint + EPSILON * direction;
	Ray shadowRay = Ray(intersectionWithEpsilon, direction);
	shadowRay.t = INFINITY;
	float lightIntensity = 0.0f;

	float tToLight = (lightSource->position.x - intersectionWithEpsilon.x) / direction.x;

#if !USEBVH
	for (int x = 0; x < sizeof(this->scene->primitives) / sizeof(this->scene->primitives[0]); x++)
	{
		this->scene->primitives[x]->CheckIntersection(&shadowRay);

		//Check if the intersection is between the original intersection and the light. If it is, return black.
		if (shadowRay.t < tToLight)
			return vec3(0.0f, 0.0f, 0.0f);
	}

#elif USEBVH
	scene->bvh->Traverse(&shadowRay, scene->bvh->rootNode, true);

	//Check if the intersection is between the original intersection and the light. If it is, return black.
	if (shadowRay.t < tToLight)
		return vec3(0.0f, 0.0f, 0.0f);
#endif

	float euclidianDistanceToLight = distance(intersectionPoint, lightSource->position);

	// I = LightColor * N. L * 1/d^2 * BRDF/PI
	return lightSource->color *
		dot(normal, direction) *
		(1 / (euclidianDistanceToLight*euclidianDistanceToLight)) *
		(material.color / PI);
	}