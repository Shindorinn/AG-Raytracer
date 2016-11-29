#include "template.h"
#include "Scene.h"
#include "Sphere.h"

Scene::Scene()
{
	printf("hallo");
	camera = new Camera();
	lights[0] = new Light(vec3(2, 5, 2), 10.0f);
	primitives[0] = new Sphere(vec3(0, 0, 2), 1.0f);
	primitives[1] = new Sphere(vec3(1, 0, 2), 0.2f);
}