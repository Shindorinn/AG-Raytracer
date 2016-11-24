#include "template.h"
#include "Scene.h"
#include "Sphere.h"

Scene::Scene()
{
	camera = Camera();
	lights[0] = new Light(vec3(2,5,2), 10.0f);

	primitives[0] = new Sphere(vec3(0, 0, 5), 5.0f);


}