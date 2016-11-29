#include "template.h"
#include "Scene.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Plane.h"

Scene::Scene()
{
	printf("hallo");
	camera = new Camera();
	lights[0] = new Light(vec3(2, 5, 2), 10.0f);
	//primitives[0] = new Sphere(vec3(0, 0, 2), 1.0f);
	//primitives[0] = new Triangle(vec3(0, 0, 2), vec3(2, 0, 2), vec3(0, 2, 2));
	primitives[0] = new Plane(vec3(2,2, 2), vec3(0, 0, 1));
}
