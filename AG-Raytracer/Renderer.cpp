#include "template.h"

#define DEBUG 1
#define USEBVH 1

Renderer::Renderer(Scene* scene, Surface* renderSurface)
{
	this->scene = scene;
	this->renderSurface = renderSurface;
	this->scene->camera->GenerateRays();
}

void Renderer::Render() {
	if (this->scene->camera->dirty()) {
		this->scene->camera->buildCamera();
		this->scene->camera->UpdateRays();
		this->scene->camera->clearDirty();
	}

//#pragma omp parallel for collapse 2
#pragma omp parallel for
	for (int y = 0; y < SCRHEIGHT; y++) {
#pragma omp parallel for
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
#pragma omp parallel for
	for (int y = 0; y < SCRHEIGHT; y++)
#pragma omp parallel for
		for (int x = 0; x < SCRWIDTH; x++)
			this->renderSurface->Plot(x, y, this->buffer[y][x]);
}

vec3 Renderer::Trace(Ray* ray, int x, int y)
{
	float smallestT = INFINITY;

#if !USEBVH
	for (int x = 0; x < sizeof(this->scene->primitives) / sizeof(this->scene->primitives[0]); x++)
	{
		if (this->scene->primitives[x]->CheckIntersection(ray) && smallestT > ray->t)
		{
			smallestT = ray->t;
			ray->hit = this->scene->primitives[x];
		}
	}
#else 
	scene->bvh->Traverse(ray, scene->bvh->rootNode);

	smallestT = ray->t;
#endif

	if (smallestT == INFINITY) {
		return vec3(0, 0, 0);
	}
	else {
		Primitive* hit = ray->hit;
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