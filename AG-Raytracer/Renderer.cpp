#include "template.h"
#include "Renderer.h"

#define DEBUG 1

Renderer::Renderer(Scene* scene, Surface* renderSurface)
{
	this->scene = scene;
	this->renderSurface = renderSurface;
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
			this->renderSurface->Plot(x, y, this->buffer[y][x]);
}


Pixel Renderer::Trace(Ray* ray, int x, int y)
{
	float smallestT = INFINITY;
	//Material mat;

	for (int x = 0; x < sizeof(this->scene->primitives) / sizeof(this->scene->primitives[0]); x++)
	{
		if (this->scene->primitives[x]->CheckIntersection(ray) && smallestT > ray->t)
		{
			smallestT = ray->t;
			//mat = this->scene->primitives[x]->material;
		}

	}
	if (ray->t == INFINITY) {
		return 0xffffff;
	}
	else {
		vec3 intersectionPoint = ray->origin + ray->t*ray->direction;

		//TODO: Take multiple lights into account (sum them).
		return 0xff0000 * DirectIllumination(intersectionPoint, glm::normalize(ray->origin - scene->lights[0]->position));
	}
}

float Renderer::DirectIllumination(vec3 intersectionPoint, vec3 surfaceNormal) {
	Ray* shadowRay = new Ray(intersectionPoint, surfaceNormal);
	for (int x = 0; (x < sizeof(this->scene->primitives) / sizeof(this->scene->primitives[0])) && (shadowRay->t == INFINITY); x++)
	{
		this->scene->primitives[x]->CheckIntersection(shadowRay);
	}
	if (shadowRay->t == INFINITY) {
		delete shadowRay;
		return 0.0f;
	}
	else {
		delete shadowRay;
		return 1.0f;
	}
}