#include "template.h"
#include "Renderer.h"

#define DEBUG 1
#define EPSILON 0.01f

Renderer::Renderer(Scene* scene, Surface* renderSurface)
{
	this->scene = scene;
	this->renderSurface = renderSurface;
	this->scene->camera->GenerateRays();
}

void Renderer::Render() {
	for (int y = 0; y < SCRHEIGHT; y++) {
		#pragma omp parallel
		#pragma omp for
		for (int x = 0; x < SCRWIDTH; x++)
		{
			vec3 colorResult = Trace(this->scene->camera->primaryRays[y*SCRWIDTH + x], x, y);

			// First convert range
			colorResult *= 256.0f;
			// Then clamp
			int r = min((int)colorResult.x, 255);
			int g = min((int)colorResult.y, 255);
			int b = min((int)colorResult.z, 255);
			// Then merge

			buffer[y][x] = ((r << 16) + (g << 8) + (b));
		}
	}

	for (int y = 0; y < SCRHEIGHT; y++)
		#pragma omp parallel
		#pragma omp for
		for (int x = 0; x < SCRWIDTH; x++)
			this->renderSurface->Plot(x, y, this->buffer[y][x]);
}

vec3 Renderer::Trace(Ray* ray, int x, int y)
{
	float smallestT = INFINITY;
	Primitive* hit;

	for (int x = 0; x < sizeof(this->scene->primitives) / sizeof(this->scene->primitives[0]); x++)
	{
		if (this->scene->primitives[x]->CheckIntersection(ray) && smallestT > ray->t)
		{
			smallestT = ray->t;
			hit = this->scene->primitives[x];
		}
	}

	if (smallestT == INFINITY) {
		return vec3(0, 0, 0);
	}
	else {
		vec3 intersectionPoint = ray->origin + smallestT*ray->direction;
		vec3 normal = hit->GetNormal(intersectionPoint);
		vec3 colorResult = vec3(0, 0, 0);
		
		if (hit->material.materialKind == Material::MaterialKind::DIFFUSE)
		{
			for (int i = 0; i < sizeof(this->scene->lights) / sizeof(this->scene->lights[0]); i++)
			{
				vec3 direction = glm::normalize(scene->lights[i]->position - intersectionPoint);

				if (dot(direction, normal) < 0)
					continue;

				colorResult += DirectIllumination(
					intersectionPoint,
					direction,
					normal,
					scene->lights[i],
					hit->material);
			}

			ray->t = INFINITY;
		}
		if (hit->material.materialKind == Material::MaterialKind::MIRROR)
		{
			ray->t = INFINITY;
			return hit->material.color * Trace(&Ray(intersectionPoint, reflect(ray->direction, normal)), x, y);
		}

		if (hit->material.materialKind == Material::MaterialKind::GLASS)
		{
			printf("glass hit");
			return this->Refract(intersectionPoint, hit);
			
		}
		return colorResult;
	}
}

vec3 Renderer::DirectIllumination(vec3 intersectionPoint, vec3 direction, vec3 normal, Light* lightSource, Material material)
{
	vec3 intersectionWithEpsilon = intersectionPoint + EPSILON * direction;
	Ray shadowRay = Ray(intersectionWithEpsilon, direction);
	shadowRay.t = INFINITY;
	float lightIntensity = 0.0f;

	float tToLight = (lightSource->position.x - intersectionWithEpsilon.x) / direction.x;

	for (int x = 0; x < sizeof(this->scene->primitives) / sizeof(this->scene->primitives[0]); x++)
	{
		this->scene->primitives[x]->CheckIntersection(&shadowRay);

		//Check if the intersection is between the original intersection and the light. If it is, return black.
		if (shadowRay.t < tToLight)
			return vec3(0.0f, 0.0f, 0.0f);
	}

	float euclidianDistanceToLight = distance(intersectionPoint, lightSource->position);

	// I = LightColor * N. L * 1/d^2 * BRDF/PI
	return lightSource->color *
		dot(normal, direction) *
		(1 / (euclidianDistanceToLight*euclidianDistanceToLight)) *
		(material.color / PI);
}

vec3 Renderer::Refract(vec3 direction, Primitive* primitive) {
	//TODO: IMPLEMENT REFRACTION
	// TODO: SPLIT INTO OUTTOINREFRACT AND VICE VERSA
	// TODO: FIND A GOOD WAY TO DETERMINE REFLECTION
	// First find reflection element and if TIR happens
	vec3 normal = primitive->GetNormal(direction);
	float incidentCos = dot(normal, direction);
	// Then do refraction ( out to in )
	float sinT2 = primitive->material.ndiv2 * (1 - incidentCos * incidentCos);
	if (sinT2 > 1.0) // TIR
		return vec3(0, 0, 0);
	float ndiv = primitive->material.ndiv;
	return ndiv * direction - (ndiv + sqrtf(1.0 - sinT2)) * normal;
	// Return both parts.
	
	// two refraction indices, n1 and n2
	// 𝐹𝑟 = 𝑅0 + (1 − 𝑅0)(1 − 𝑐𝑜𝑠𝜃)^5
	// where 𝑅0 = (	(𝑛1−𝑛2) / (𝑛1 + 𝑛2) )^2
}