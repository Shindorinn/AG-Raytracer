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
			buffer[x][y] = Trace(this->scene->camera->primaryRays[y*SCRWIDTH + x], x, y);
		}

#if DEBUG == 1
	printf("Tracing is done. \n");
#endif
}


Pixel Renderer::Trace(Ray* ray, int x, int y)
{
	for (int x = 0; x < sizeof(this->scene->primitives) / sizeof(this->scene->primitives[0]); x++)
	{
		this->scene->primitives[x]->CheckIntersection(ray);

	}
	if (ray->t == INFINITY)
		return 0xffffff;
	else
		return 0xff0000;
}