#include "template.h"
#include "Renderer.h"

#define DEBUG 1

Renderer::Renderer(Scene* scene)
{
#if DEBUG == 1
	printf("1doet dit Überhaupt dingen \n");
#endif
	this->scene = scene;
	this->scene->camera->GenerateRays();

	for (int y = 0; y < SCRHEIGHT; y++)
		for (int x = 0; x < SCRWIDTH; x++)
		{
			Trace(this->scene->camera->primaryRays[y*SCRWIDTH + x]);
		}

#if DEBUG == 1
	printf("Tracing is done. \n");
#endif
}


vec3 Renderer::Trace(Ray* ray)
{
	for (int x = 0; x < sizeof(this->scene->primitives) / sizeof(this->scene->primitives[0]); x++)
	{
		this->scene->primitives[x]->CheckIntersection(ray);
	
	}

	return vec3(1, 1, 1);
}