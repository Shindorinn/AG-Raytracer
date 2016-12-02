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
	//this->scene->camera->GenerateRays();
	for (int y = 0; y < SCRHEIGHT; y++) {
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

		if (hit->material.materialKind == Material::MaterialKind::DIFFUSE)
		{
			vec3 colorResult = vec3(0, 0, 0);

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
			return colorResult;
		}
		if (hit->material.materialKind == Material::MaterialKind::MIRROR)
		{
			//TODO: IMPLEMENT REFLECTION, ff zonder new, gewoon direct r.dir meegeven wellicth? 

			ray->t = INFINITY;
			return hit->material.color * Trace(new Ray(intersectionPoint, reflect(ray->direction, normal)), x, y);

		}

		if (hit->material.materialKind == Material::MaterialKind::GLASS)
		{
			printf("glass hit");
			//TODO: IMPLEMENT REFRACTION
		}


	}
}

vec3 Renderer::DirectIllumination(vec3 intersectionPoint, vec3 direction, vec3 normal, Light* lightSource, Material material)
{
	vec3 intersectionWithEpsilon = intersectionPoint + EPSILON * direction;
	Ray* shadowRay = new Ray(intersectionWithEpsilon, direction);
	float lightIntensity = 0.0f;

	for (int x = 0; (x < sizeof(this->scene->primitives) / sizeof(this->scene->primitives[0])) && (shadowRay->t == INFINITY); x++)
	{
		this->scene->primitives[x]->CheckIntersection(shadowRay);
	}

	if (shadowRay->t != INFINITY) {
		delete shadowRay;
		return vec3(0.0f, 0.0f, 0.0f);
	}
	else {
		float euclidianDistance = distance(intersectionPoint, lightSource->position);
		//float euclidianDistance = shadowRay->t;
		delete shadowRay;

		// I = LightColor * N. L * 1/d^2 * BRDF/PI
		return lightSource->color *
			dot(normal, direction) *
			(1 / (euclidianDistance*euclidianDistance)) *
			(material.color / PI);
	}
}

//TODO: delete weghalen