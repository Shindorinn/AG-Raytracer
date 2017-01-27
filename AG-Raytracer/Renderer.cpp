﻿#include "template.h"

#define DEBUG 1
#define EPSILON 0.0001f
#define INVPI 0.31830988618379067153776752674503f

#define Psurvival 0.8f

#define USEBVH 0

#define MAXRAYDEPTH 5

#define UseRR 0

int pixel = 1;

Renderer::Renderer(Scene* scene, Surface* renderSurface)
{
	this->scene = scene;
	this->renderSurface = renderSurface;
	this->scene->camera->GenerateRays();
	this->frameCount = 0;
}

//Int, so it can return the total pixel values summed
int Renderer::Render() {

	int pixelCount = 1;

	//Increase frameCount, which is used for averaging multiple frames of path tracing.
	frameCount++;

	//#pragma omp parallel for
	for (int y = 0; y < SCRHEIGHT; y++) {
		//printf("Current y:%i% \n", y);
//#pragma omp parallel for
		for (int x = 0; x < SCRWIDTH; x++)
		{
			if (x == 632 && y == 422)
				printf("test");
			vec3 colorResult;
			//	if (x < SCRWIDTH / 2)
			colorResult = Sample(this->scene->camera->primaryRays[y*SCRWIDTH + x], 0);
			//else
			//colorResult = BasicSample(this->scene->camera->primaryRays[y*SCRWIDTH + x], 0);

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

			if (r < 0 || g < 0 || b < 0)
				printf("dingen zijn <0");

			//printf("r: %f%,g: %f%,b:%f% \n", r, g, b);

			// Then clamp the newly calculated float values (they may be way above 255, when something is very bright for example).
			int nextR = min((int)r, 255);
			int nextG = min((int)g, 255);
			int nextB = min((int)b, 255);

			pixelCount += (int)r + (int)g + (int)b; //nextR + nextG + nextB;

			// Then merge it to the buffer.
			buffer[y][x] = (nextR << 16) + (nextG << 8) + (nextB);

			pixel++;
		}
	}
	//#pragma omp parallel for
	for (int y = 0; y < SCRHEIGHT; y++)
		//#pragma omp parallel for
		for (int x = 0; x < SCRWIDTH; x++)
			this->renderSurface->Plot(x, y, this->buffer[y][x]);

	pixel = 0;
	//TODO: Use this for testing?
//	this->renderSurface->Plot(600, 350, 0xffffff);
	return pixelCount;

}

vec3 Renderer::Sample(Ray* ray, int depth, bool secondaryRay)
{
	/*
	Over Russian Roulette:

	Pak geen vaste waarde voor kans, maar pak (r+g+b)/3, en dit cappen tussen 0 en 1.
	Verder: je DirectLight moet je altijd doen, 100%, dus die ook ZEKER NIET delen door je kans.
	Je moet wel de RandomLights delen door die kans, maar dus NIET je totaal.
	*/
#if UseRR
	float a = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	//Russian Roulette; check if we need to kill a ray.
	if (a > Psurvival && secondaryRay)
	{
		return vec3(0);
	}
#else

	if (depth > MAXRAYDEPTH)
	{
		return vec3(0);
	}
#endif

	// trace ray
	vec3 intersect = Trace(ray);

	// terminate if ray left the scene
	if (ray->t == INFINITY)
	{
		return vec3(0);
	}

	Entity* hit = ray->hit;

	if (hit->isLight)
	{
		//This is to check if the ray is an indirect illumination one.
		if (secondaryRay)
			return vec3(0);
		else
			return static_cast<Light*>(hit)->color;
	}

	Primitive* primitiveHit = static_cast<Primitive*>(hit);

	vec3 normal = primitiveHit->GetNormal(intersect);


	if (primitiveHit->material.materialKind == Material::MaterialKind::MIRROR)
	{
		// continue in fixed direction
		Ray r(intersect, reflect(ray->direction, normal));
		return primitiveHit->material.color * Sample(&r, depth + 1, false);
	}

	// continue in random direction
	vec3 R = CosineWeightedDiffuseReflection(normal);

	//This random ray is used for the indirect lighting.
	Ray newRay = Ray(intersect + R * EPSILON, R);

	vec3 directIllumination = DirectSampleLights(intersect, normal, primitiveHit->material);

	//TODO: Albedo setting maybe.
	vec3 BRDFIndirect = primitiveHit->material.color * INVPI;


	float PDF = 1 / (2 * PI); //dot(normal, R) / PI;//
	float dotdot = dot(normal, R);
	vec3 Ei = Sample(&newRay, depth + 1, true) * dot(normal, R) / PDF; // irradiance
	vec3 indirectIllumination = BRDFIndirect * Ei;

#if UseRR
	return vec3(indirectIllumination.x / Psurvival, indirectIllumination.y / Psurvival, indirectIllumination.z / Psurvival) + directIllumination;

#else
	vec3 result = indirectIllumination + directIllumination;
	if (result.x < 0 || result.y < 0 || result.z < 0)
		printf("smaller 0");
	return indirectIllumination + directIllumination;

#endif
}

vec3 Renderer::DirectSampleLights(vec3 intersect, vec3 normal, Material material)
{
	//TODO: Don't hardcode the 2, but get #lights.
	int lightIndex = rand() % 2;
	Triangle* lightTri = this->scene->lights[lightIndex]->tri;

	//TODO: Check if this code could be made faster.  || ANSWER: It's okay, but could try 1-a and 1-b, if a+b > 1. In practice, however, this shouldn't have that big of an effect.
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

	float cos_i = dot(L, normal);

	if ((cos_o <= 0) || (cos_i <= 0)) return vec3(0);

	Ray r = Ray(intersect + EPSILON * L, L);

	r.hit = 0;

	//If our ray to a light hits something on its path towards the light, we can't see light, so we will return black.
	Trace(&r);

	if (r.hit)
		//TODO: Check of geraakte primitive de lightTri is. Dan optimalisatie: Shadowrays, midner werk, sneller
		if (!r.hit->isLight)
			//Als we licht geraakt hebben niks ertussen, dus prima.
			return vec3(0);

	Entity* hit = r.hit;
	Light* lightHit = static_cast<Light*>(hit);

	//TODO: Check if this should indeed just be "Material.color".
	vec3 BRDF = material.color * INVPI;
	float solidAngle = (cos_o * this->scene->lights[lightIndex]->area) / (dist*dist);


	vec3 result = BRDF * 2.0f * this->scene->lights[lightIndex]->color * solidAngle * cos_i;
	if (result.x < 0 || result.y < 0 || result.z < 0)
		printf("smaller in direct illu 0");

	//TODO: Don't hardcode the 2.0f, but get #lights.
	return BRDF * 2.0f * this->scene->lights[lightIndex]->color * solidAngle * cos_i;
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
		if (dot(static_cast<Light*>(hit)->tri->normal, ray->direction) > 0)
			return vec3(0);
		else
			return static_cast<Light*>(hit)->color;

	Primitive* primitiveHit = static_cast<Primitive*>(hit);

	vec3 normal = primitiveHit->GetNormal(intersect);

	// continue in random direction
	vec3 R = CosineWeightedDiffuseReflection(normal);
	Ray newRay = Ray(intersect + R * EPSILON, R);

	//float BRDF = primitiveHit->material.albedo / PI;
	vec3 BRDF = primitiveHit->material.color * INVPI;

	vec3 Ei = BasicSample(&newRay, depth + 1) * dot(normal, R); // irradiance

	return PI * 2.0f * BRDF * Ei;
}

glm::uint Renderer::RandomInt(glm::uint * seed)
{
	// Marsaglia Xor32; see http://excamera.com/sphinx/article-xorshift.html
	// also see https://github.com/WebDrake/xorshift/blob/master/xorshift.c for higher quality variants
	*seed ^= *seed << 13;
	*seed ^= *seed >> 17;
	*seed ^= *seed << 5;
	return *seed;
}


float Renderer::RandomFloat(glm::uint * seed)
{
	return RandomInt(seed) * 2.3283064365387e-10f;
}

vec3 Renderer::CosineWeightedDiffuseReflection(vec3 normal)
{
	glm::uint  seed1 = ((uint)frameCount + pixel) * 160481219 * 49979687;
	glm::uint  seed2 = ((uint)frameCount + pixel) * 32452867 * 67867979;

	//float r0 = RandomFloat(&seed1);
	//float r1A = RandomFloat(&seed2);
	//	printf("random1: %f% ", randomfloat1);
		//printf("random2: %f% ", randomfloat2);

	//WHAT OTHER PEOPLE DID (AND WORKS):  http://i.imgur.com/tfpLGce.png
	//WHAT JACCO DID: http://i.imgur.com/rSLuSxD.png

	//NOTE: THE RANDOM WITH SEEDS YIELDS ENTIRELY DIFFERENT RESULTS. SOMETHING IS WEIRD/BROKEN.

	float r0 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX), r1A = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float r2s = sqrt(r0);
	float r1 = 2 * PI * r1A;
	float x = r2s * cos(r1);
	float y = r2s * sin(r1);

	vec3 dir = vec3(x, y, sqrt(1 - r0));

	//vec3 randomDir = normalize(vec3(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));
	//Note: this direction below isn't random, but I think it should work.
	vec3 randomDir = abs(normal.x) > 0.1f ? vec3(0.0f, 1.0f, 0.0f) : vec3(1.0f, 0.0f, 0.0f);

	vec3 t = cross(randomDir, normal);
	vec3 b = cross(normal, t);

	mat3 tangentSpace = mat3(t, b, normal);
	//mat3 test = mat3(t.x, b.x, normal.x, t.y, b.y, normal.y, t.z, b.z, normal.z);

	vec3 manualDirNotNormalized = vec3(dir.x * t.x + dir.x*t.y + dir.x * t.z, dir.y * b.x + dir.y* b.y + dir.y * b.z, dir.z * normal.x + dir.z * normal.y + dir.z * normal.z);

	//transformedDir is what we used before. Looks almost right, but I'm almost certain it is not.
	vec3 transformedDir = tangentSpace * dir;

	if (dot(normalize(manualDirNotNormalized), normal) < 0)
		printf("this should never happen but happens. Fuck.");
	return normalize(manualDirNotNormalized);
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
		if (this->scene->entities[x]->CheckIntersection(ray) && smallestT > ray->t)
		{
			smallestT = ray->t;
			ray->hit = this->scene->entities[x];
		}
	}

#else 
	scene->bvh->Traverse(ray, scene->bvh->rootNode);
	smallestT = ray->t;

	//Lights are not included in the BVH.

	//TODO: Fix this hardcoding.
	for (int x = 12; x < 14; x++)
	{
		if (this->scene->entities[x]->CheckIntersection(ray) && smallestT > ray->t)
		{
			smallestT = ray->t;
			ray->hit = this->scene->entities[x];
		}
		}

	smallestT = ray->t;
#endif

	if (smallestT == INFINITY)
	{
		return vec3(0);
	}
	else
	{
		//Set t back, this is needed for the pathtracing code which checks if we need to return black (occlusion toward light)
		ray->t = smallestT;

		vec3 intersectionPoint = ray->origin + smallestT*ray->direction;
		//vec3 normal = hit->GetNormal(intersectionPoint);
		vec3 colorResult = vec3(0, 0, 0);

		return intersectionPoint;
	}
	}