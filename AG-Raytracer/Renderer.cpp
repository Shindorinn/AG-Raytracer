#include "template.h"
#include "Renderer.h"

#define DEBUG 1

Renderer::Renderer(Scene* scene, Surface* renderSurface)
{
	this->scene = scene;
	this->renderSurface = renderSurface;

	printf("\n Camera Direction : (%f, %f, %f, %f) \n", this->scene->camera->viewDirection.x, this->scene->camera->viewDirection.y, this->scene->camera->viewDirection.z, this->scene->camera->viewDirection.w);

	this->scene->camera->GenerateRays();

	/*for (int y = 0; y < SCRHEIGHT; y++) {
		for (int x = 0; x < SCRWIDTH; x++)
		{
			buffer[x][y] = Trace(this->scene->camera->primaryRays[y*SCRWIDTH + x], x, y);
		}
	}*/
}

void Renderer::Render() {

	for (int y = 0; y < SCRHEIGHT; y++) {
		for (int x = 0; x < SCRWIDTH; x++)
		{
			buffer[y][x] = Trace(this->scene->camera->primaryRays[y*SCRWIDTH + x], x, y);
		}
	}

	for (int y = 0; y < SCRHEIGHT; y++)
		for (int x = 0; x < SCRWIDTH; x++)
			this->renderSurface->Plot(x, y, this->buffer[x][y]);
}


Pixel Renderer::Trace(Ray* ray, int x, int y)
{
	for (int x = 0; x < sizeof(this->scene->primitives) / sizeof(this->scene->primitives[0]); x++)
	{
		this->scene->primitives[x]->CheckIntersection(ray);

	}
	if (ray->t == INFINITY) {
		return 0xffffff;
	}
	else {
		vec3 intersectionPoint = ray->origin + ray->t*ray->direction;
		return 0xff0000 * DirectIllumination(intersectionPoint, glm::normalize(ray->origin - scene->lights[0]->position));
	}
}

float Renderer::DirectIllumination(vec3 intersectionPoint, vec3 surfaceNormal) {
	Ray* shadowRay = new Ray(intersectionPoint, surfaceNormal);
	for (int x = 0; (x < sizeof(this->scene->primitives) / sizeof(this->scene->primitives[0] )) && (shadowRay->t == INFINITY); x++)
	{
		this->scene->primitives[x]->CheckIntersection(shadowRay);
	}
	if (shadowRay->t == INFINITY) {
		return 0.0f;
	}
	else {
		return 1.0f;
	}
}