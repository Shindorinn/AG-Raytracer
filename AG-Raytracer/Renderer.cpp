#include "template.h"

#define DEBUG 1
#define EPSILON 0.005f
#define INVPI 0.31830988618379067153776752674503f

#define USEBVH 0

#define MAXRAYDEPTH 10
#define UseRR 1

glm::uint seed1 = 6217 * 57089;

Renderer::Renderer(Scene* scene, Surface* renderSurface)
{

	this->scene = scene;
	this->renderSurface = renderSurface;
	this->scene->camera->GenerateRays();
	this->frameCount = 0;
	this->pixelNumber = 1;
	this->numberOfLights = sizeof(this->scene->lights) / sizeof(this->scene->lights[0]);
}

//Int, so it can return the total pixel values summed
int Renderer::Render() {

	int pixelCount = 0;

	//Increase frameCount, which is used for averaging multiple frames of path tracing.
	frameCount++;

#pragma omp parallel for
	for (int y = 0; y < SCRHEIGHT; y++) {

#pragma omp parallel for
		for (int x = 0; x < SCRWIDTH; x++)
		{
			if (x == 680 && y == 139)
				printf("test");
			vec3 colorResult;
			if (x < SCRWIDTH / 2)
				colorResult = Sample(this->scene->camera->primaryRays[y*SCRWIDTH + x], 0);
			else
				colorResult = BasicSample(this->scene->camera->primaryRays[y*SCRWIDTH + x], 0);

			// First convert range
			colorResult *= 256.0f;

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

			// Then clamp the newly calculated float values (they may be way above 255, when something is very bright for example).
			int nextR = min((int)r, 255);
			int nextG = min((int)g, 255);
			int nextB = min((int)b, 255);

			//Gamma correction.
			nextR = pow((float)nextR / 255, 1 / 2.2f) * 255;
			nextG = pow((float)nextG / 255, 1 / 2.2f) * 255;
			nextB = pow((float)nextB / 255, 1 / 2.2f) * 255;

			pixelCount += (int)r + (int)g + (int)b;

			// Then merge it to the buffer.
			buffer[y][x] = (nextR << 16) + (nextG << 8) + (nextB);

			this->pixelNumber++;
		}
	}
	//#pragma omp parallel for
	for (int y = 0; y < SCRHEIGHT; y++)
		//#pragma omp parallel for
		for (int x = 0; x < SCRWIDTH; x++)
			this->renderSurface->Plot(x, y, this->buffer[y][x]);

	this->pixelNumber = 1;

	return pixelCount;
}


vec3 Renderer::SampleMIS(Ray* ray)
{
#if !UseRR
	if (depth > MAXRAYDEPTH)
	{
		return vec3(0);
	}
#endif

	int depth = 0;

	bool secondary = false;
	vec3 T = vec3(1); vec3 E = vec3(0);
	while (true)
	{
		// trace ray
		vec3 intersect = Trace(ray);

		// terminate if ray left the scene
		if (ray->t == INFINITY)
		{
			break;
		}

		Entity* hit = ray->hit;

		if (hit->isLight)
		{
			////This is to check if the ray is an indirect illumination one.
			//if (secondary)
			//	break;
			//else
			//{
			//	E += static_cast<Light*>(hit)->color;
			//	break;
			//}

			{
				Light* light = static_cast<Light*>(hit);

				vec3 L = intersect - ray->origin;
				float dist = length(L);
				L /= dist;

				float brdfPDF = dot(light->tri->normal, -L) * INVPI;
				float cos_o = dot(-L, light->tri->normal);
				float solidAngle = (cos_o * light->area) / (dist*dist);
				float lightPDF = 1 / solidAngle;


				float misPDF = lightPDF + brdfPDF;

				E += T* (light->color / misPDF);
				break;
			}

		}

		Primitive* primitiveHit = static_cast<Primitive*>(hit);

#if UseRR
		float a = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		float pSurvive = clamp(max(max(primitiveHit->material.color.r, primitiveHit->material.color.g), primitiveHit->material.color.b), 0.0f, 1.0f);

		//Russian Roulette; check if we need to kill a ray.
		if (a > pSurvive && secondary)
		{
			break;
		}
#endif

		vec3 normal = primitiveHit->GetNormal(intersect);

		normal = dot(normal, ray->direction) <= 0.0f ? normal : normal * (-1.0f);

#pragma region 
		if (primitiveHit->material.materialKind == Material::MaterialKind::MIRROR)
		{
			secondary = false;

			// continue in fixed direction
			vec3 newDirection = reflect(ray->direction, normal);
			Ray r(intersect + newDirection * EPSILON, newDirection);

			ray = &r;
			E += T* primitiveHit->material.color;

			//return primitiveHit->material.color * Sample(&r, depth + 1, false);
		}

		if (primitiveHit->material.materialKind == Material::MaterialKind::GLASS)
		{
			secondary = false;

			bool outside = true;
			float distPositionOrigin = distance(ray->origin, primitiveHit->position);
			//Check if we are inside or outside.
			if (distPositionOrigin < static_cast<Sphere*>(hit)->radius)
				outside = false;

			vec3 newDirection = Refract(outside, ray->direction, normal);
			Ray r(intersect + newDirection * EPSILON, newDirection);

			ray = &r;
			E += T* primitiveHit->material.color;
			//return primitiveHit->material.color * Sample(&r, depth + 1, false);
		}
#pragma endregion Reflection/Refraction

		E += T * DirectSampleLights(intersect, normal, primitiveHit->material, true);

		// continue in random direction
		vec3 R = CosineWeightedDiffuseReflection(normal);

		//This random ray is used for the indirect lighting.
		Ray newRay = Ray(intersect + R * EPSILON, R);

		ray = &newRay;

		vec3 BRDFIndirect = primitiveHit->material.color * INVPI;

		float PDF = dot(normal, R) / PI;

		//vec3 Ei = Sample(&newRay, depth + 1, true) * dot(normal, R) / PDF; // irradiance
		vec3 indirectIllumination = BRDFIndirect * (dot(normal, R) / PDF);

		depth++;
		secondary = true;

#if UseRR
		indirectIllumination /= pSurvive;

#else
		//vec3 result = indirectIllumination + directIllumination;
		//if (result.x < 0 || result.y < 0 || result.z < 0)
		//	printf("smaller 0");

		//return indirectIllumination + directIllumination;

#endif
		T *= indirectIllumination;
	}
	return E;
}

vec3 Renderer::Sample(Ray* ray, int depth, bool secondaryRay)
{
#if !UseRR
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
			//Check if our primary ray hits the light on the top side (light only shines down in our code).
			if (dot(static_cast<Light*>(hit)->tri->normal, ray->direction) > 0)
				return vec3(0);
			else
				return static_cast<Light*>(hit)->color;
	}

	Primitive* primitiveHit = static_cast<Primitive*>(hit);

#if UseRR
	float a = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	float pSurvive = clamp(max(max(primitiveHit->material.color.r, primitiveHit->material.color.g), primitiveHit->material.color.b), 0.0f, 1.0f);

	//Russian Roulette; check if we need to kill a ray.
	if (a > pSurvive && secondaryRay)
	{
		return vec3(0);
	}
#endif

	vec3 normal = primitiveHit->GetNormal(intersect);

	normal = dot(normal, ray->direction) <= 0.0f ? normal : normal * (-1.0f);

#pragma region 
	if (primitiveHit->material.materialKind == Material::MaterialKind::MIRROR)
	{
		// continue in fixed direction
		vec3 newDirection = reflect(ray->direction, normal);
		Ray r(intersect + newDirection * EPSILON, newDirection);
		return primitiveHit->material.color * Sample(&r, depth + 1, false);
	}

	if (primitiveHit->material.materialKind == Material::MaterialKind::GLASS)
	{
		bool outside = true;
		float distPositionOrigin = distance(ray->origin, primitiveHit->position);
		//Check if we are inside or outside.
		if (distPositionOrigin < static_cast<Sphere*>(hit)->radius)
			outside = false;

		vec3 newDirection = Refract(outside, ray->direction, normal);
		Ray r(intersect + newDirection * EPSILON, newDirection);
		return primitiveHit->material.color * Sample(&r, depth + 1, false);
	}
#pragma endregion Reflection/Refraction

	vec3 directIllumination = DirectSampleLights(intersect, normal, primitiveHit->material);

	// continue in random direction
	vec3 R = CosineWeightedDiffuseReflection(normal);

	//This random ray is used for the indirect lighting.
	Ray newRay = Ray(intersect + R * EPSILON, R);

	vec3 BRDFIndirect = primitiveHit->material.color * INVPI;

	float PDF = dot(normal, R) / PI;

	vec3 Ei = Sample(&newRay, depth + 1, true) * dot(normal, R) / PDF; // irradiance
	vec3 indirectIllumination = BRDFIndirect * Ei;

#if UseRR
	return (indirectIllumination / pSurvive) + directIllumination;

#else
	vec3 result = indirectIllumination + directIllumination;
	if (result.x < 0 || result.y < 0 || result.z < 0)
		printf("smaller 0");

	return indirectIllumination + directIllumination;

#endif
}

vec3 Renderer::DirectSampleLights(vec3 intersect, vec3 normal, Material material, bool isMIS)
{
	int lightIndex = rand() % numberOfLights;
	Triangle* lightTri = this->scene->lights[lightIndex]->tri;

	float a = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	if (a + b > 1)
	{
		a = 1 - a;
		b = 1 - b;
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

	r.t = length(point - (intersect + 2 * EPSILON * L));

	//If our ray to a light hits something on its path towards the light, we can't see light, so we will return black.
	//This is a shadow ray, so we can speed up tracing (return if we hit something).
	vec3 resultOfShadowTrace = Trace(&r, true);

	//The results acts as a bool here. vec3(0) states that the light is occluded.
	if (resultOfShadowTrace == vec3(0))
		return vec3(0);

	Light* lightHit = this->scene->lights[lightIndex];

	vec3 BRDF = material.color * INVPI;
	float solidAngle = (cos_o * this->scene->lights[lightIndex]->area) / (dist*dist);
	float lightPDF = 1 / solidAngle;

	vec3 result;
	if (isMIS)
	{
		float brdfPDF = dot(normal, L) * INVPI;
		float misPDF = lightPDF + brdfPDF;
		result = BRDF * (float)numberOfLights * this->scene->lights[lightIndex]->color * (cos_i / misPDF);
	}

	else
	{
		result = BRDF * (float)numberOfLights * this->scene->lights[lightIndex]->color * (cos_i / lightPDF);
	}

	return result;
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
		return vec3(0);
	}

	Entity* hit = ray->hit;

	// terminate if we hit a light source
	if (hit->isLight)
	{
		if (dot(static_cast<Light*>(hit)->tri->normal, ray->direction) > 0)
			return vec3(0);
		else
			return static_cast<Light*>(hit)->color;
	}

	Primitive* primitiveHit = static_cast<Primitive*>(hit);

	vec3 normal = primitiveHit->GetNormal(intersect);

	// continue in random direction
	vec3 R = DiffuseReflection(normal);
	Ray newRay = Ray(intersect + R * EPSILON, R);

	vec3 BRDF = primitiveHit->material.color * INVPI;

	float PDF = 1 / (2.0*PI);
	vec3 Ei = BasicSample(&newRay, depth + 1) * dot(normal, R); // irradiance

	return BRDF * Ei / PDF;
}

#pragma region 
glm::uint Renderer::TauStep(int s1, int s2, int s3, glm::uint M, glm::uint* seed)
{
	glm::uint b = (((*seed << s1) ^ *seed) >> s2);
	*seed = (((*seed & M) << s3) ^ b);
	return *seed;
}


glm::uint Renderer::HQIRand(glm::uint* seed)
{
	uint z1 = TauStep(13, 19, 12, 429496729, seed);
	uint z2 = TauStep(2, 25, 4, 4294967288, seed);
	uint z3 = TauStep(3, 11, 17, 429496280, seed);
	uint z4 = 1664525 * *seed + 1013904223;
	return z1 ^ z2 ^ z3 ^ z4;
}


glm::uint Renderer::SeedRandom(glm::uint s)
{
	uint seed = s * 1099087573;
	seed = HQIRand(&seed);
	return seed;
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
#pragma endregion RandomStuff


vec3 Renderer::DiffuseReflection(vec3 normal)
{
	//TODO: andere seed dingen
	float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX), r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float term1 = 2 * PI * r1;
	float term2 = 2 * sqrt(r2 * (1 - r2));
	vec3 R = vec3(cos(term1) * term2, sin(term1) * term2, 1 - 2 * r2);
	if (R.z < 0) R.z = -R.z;

	vec3 n = normal;
	vec3 axis = fabs(n.x) > 0.1f ? vec3(0.0f, 1.0f, 0.0f) : vec3(1.0f, 0.0f, 0.0f);
	vec3 t = normalize(cross(axis, n));
	vec3 b = cross(n, t);

	vec3 result = normalize(R.x * t + R.y * b + R.z*n);
	return result;
}

vec3 Renderer::CosineWeightedDiffuseReflection(vec3 normal)
{
	/*float r1 = 2.0f * PI * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
	float r2 = 1;
	while (r2 == 1)
		r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);*/

	float r1 = 2.0f * PI * RandomFloat(&seed1);
	float r2 = 1;
	while (r2 == 1)
		r2 = RandomFloat(&seed1);

	float r2s = sqrt(r2);
	vec3 n = normal;
	vec3 axis = fabs(n.x) > 0.1f ? vec3(0.0f, 1.0f, 0.0f) : vec3(1.0f, 0.0f, 0.0f);
	vec3 t = normalize(cross(axis, n));
	vec3 b = cross(n, t);

	vec3 new_dir = normalize(t  * cos(r1) * r2s + b* sin(r1) * r2s + n * sqrt(1.0f - r2));

	return new_dir;
}

vec3 Renderer::Refract(bool inside, vec3 D, vec3 N)
{
	float n1 = inside ? 1.2f : 1, n2 = inside ? 1 : 1.2f;
	float eta = n1 / n2, cosi = dot(-D, N);
	float cost2 = 1.0f - eta * eta * (1 - cosi * cosi);
	vec3 R = reflect(D, N);
	if (cost2 > 0)
	{
		float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float a = n1 - n2, b = n1 + n2, R0 = (a * a) / (b * b), c = 1 - cosi;
		float Fr = R0 + (1 - R0) * (c * c * c * c * c);
		if (r1 > Fr) R = eta * D + ((eta * cosi - sqrt(fabs(cost2))) * N);
	}
	return R;
}

vec3 Renderer::Trace(Ray* ray, bool isShadowRay)
{
	float smallestT = INFINITY;

#if !USEBVH
	if (!isShadowRay)
	{
		for (int x = 0; x < sizeof(this->scene->entities) / sizeof(this->scene->entities[0]); x++)
		{
			if (this->scene->entities[x]->CheckIntersection(ray) && smallestT > ray->t)
			{
				smallestT = ray->t;
				ray->hit = this->scene->entities[x];
			}
		}

	}
	else
	{
		//The from the point to the light.
		float tToLight = ray->t;

		for (int x = 0; x < sizeof(this->scene->entities) / sizeof(this->scene->entities[0]); x++)
		{
			if (this->scene->entities[x]->CheckIntersection(ray))
				if (ray->t < tToLight)
					return vec3(0);
		}
		return vec3(1);
	}


#else 
	if (!isShadowRay)
	{
		scene->bvh->Traverse(ray, scene->bvh->rootNode);
		smallestT = ray->t;

		//Lights are not included in the BVH.

		//TODO: Fix this hardcoding.
		for (int x = 4058; x < 4060; x++)
		{
			if (this->scene->entities[x]->CheckIntersection(ray) && smallestT > ray->t)
			{
				smallestT = ray->t;
				ray->hit = this->scene->entities[x];
			}
		}
	}

	else
	{
		float tToLight = ray->t;
		ray->t = INFINITY;
		scene->bvh->Traverse(ray, scene->bvh->rootNode, true);

		if (ray->t < tToLight)
			return vec3(0);

		//TODO: go through lights as well.
		return vec3(1);
	}
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
		vec3 colorResult = vec3(0, 0, 0);

		return intersectionPoint;
	}
}