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
		//printf("Current y:%i% \n", y);
#pragma omp parallel for
		for (int x = 0; x < SCRWIDTH; x++)
		{
			vec3 colorResult = Sample(this->scene->camera->primaryRays[y*SCRWIDTH + x], 0);

			// First convert range
			colorResult *= 256.0f;
			//printf("r: %f%, g: %f%, b: %f% \n", colorResult.r, colorResult.g, colorResult.b);

			//Put our newly calculated values in the accumulator.
			accumulator[y][x].r += colorResult.r;
			accumulator[y][x].g += colorResult.g;
			accumulator[y][x].b += colorResult.b;

			//Now, we can use the values stored in the accumulator to calculate the correct pixel value to draw.
			//We first need to calculate correct float values, and then clamp.

			float r = accumulator[y][x].r / static_cast<float>(frameCount);
			float g = accumulator[y][x].g / static_cast<float>(frameCount);
			float b = accumulator[y][x].b / static_cast<float>(frameCount);

			// Then clamp the newly calculated float values (they may be way above 255, when something is very bright for example).
			int nextR = min((int)r, 255);
			int nextG = min((int)g, 255);
			int nextB = min((int)b, 255);

			pixelCount += nextR + nextG + nextB;

			// Then merge it to the buffer.
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

vec3 Renderer::Sample(Ray* ray, int depth, bool secondaryRay)
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
	{
		//if (secondaryRay)
		//	return vec3(0);
		//else
		return static_cast<Light*>(hit)->color;
	}

	Primitive* primitiveHit = static_cast<Primitive*>(hit);

	vec3 normal = primitiveHit->GetNormal(intersect);

	// continue in random direction
	vec3 R = CosineWeightedDiffuseReflection(normal);
	Ray newRay = Ray(intersect, R);

	//This ray is for the indirect stuff, direct ding gaat naar lamp toe en moet niet lichtkleur returnen (err, andersom dus).

	//UPDATE: wat we moeten doen, 2 verschillende Rays heel anders behandelen in je Sample.    //Je ray direct naar je licht toe returnt licht.color, je andere ray moet bij raken licht zwart returnen.
	//Je licht 

	//Hmm, heb je met  eneRay+andereRay  (alsin, licht optellen) niet dat je teveel licht gaat tellen? Of heb je dat juist niet, door je return BLACK als je ray een complete random ray was?

	// update throughput



	//This code is for directly sampling the lights.
	//TODO: Don't hardcode the 2, but get #lights.
	int lightIndex = rand() % 2;
	Triangle* lightTri = this->scene->lights[lightIndex]->tri;

	float a = 1.0; float b = 1.0;
	while (a + b > 1)
	{
		a = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}
	vec3 point = lightTri->v0 + a*(lightTri->v1 - lightTri->v0) + b*(lightTri->v2 - lightTri->v0);

	vec3 L = point - intersect;
	float dist = length(L);
	L /= dist;

	float cos_o = dot(-L, lightTri->normal);

	//TODO: check if normal should indeed be used here.
	float cos_i = dot(L, normal);

	float multiplier = 1.0f;
	if ((cos_o <= 0) || (cos_i <= 0)) multiplier = 0.0f;

	Ray r = Ray(intersect + EPSILON * L, L);
	//r.t = dist - 2 * EPSILON;

	//If our ray to a light hits something on its path towards the light, we can't see light, so we will multiply directIllumination with 0.
	Trace(&r);
	if (r.t + EPSILON * dist < dist)
		multiplier = 0.0f;

	//(not true anymore)  //If we reach this code, we know that the light is visible.
	vec3 BRDF = vec3(primitiveHit->material.color.r / PI, primitiveHit->material.color.g / PI, primitiveHit->material.color.b / PI);
	float solidAngle = (cos_o * this->scene->lights[lightIndex]->area) / (dist*dist);


	//TODO: Don't hardcode the 2, but get #lights.
	vec3 directIllumination = BRDF * 2.0f * this->scene->lights[lightIndex]->color * solidAngle * cos_i * multiplier;


	//end of direct sampling code
	//Sum both direct and indirect lighting...






	//float BRDF = primitiveHit->material.albedo / PI;
	vec3 BRDFIndirect = vec3(primitiveHit->material.color.r / PI, primitiveHit->material.color.g / PI, primitiveHit->material.color.b / PI);

	vec3 indirectIllumination = Sample(&newRay, depth + 1, true) * dot(normal, R) * PI * 2.0f * BRDFIndirect; // irradiance


	return directIllumination + indirectIllumination;
	//return PI * 2.0f * BRDFIndirect *;
}

//This is the "old", slow, noisy sampling function. But it works, so it's useful for testing.
vec3 Renderer::BasicSample(Ray* ray, int depth)
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

	//float BRDF = primitiveHit->material.albedo / PI;
	vec3 BRDF = vec3(primitiveHit->material.color.r / PI, primitiveHit->material.color.g / PI, primitiveHit->material.color.b / PI);

	vec3 Ei = BasicSample(&newRay, depth + 1) * dot(normal, R); // irradiance

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