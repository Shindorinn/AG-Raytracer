#include "template.h"
#include "Scene.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Plane.h"

Scene::Scene()
{
	printf("hallo");
	camera = new Camera();
	//lights[0] = new Light(vec3(2, 5, 2), 10.0f);
	lights[0] = new Light(vec3(0, 2, -10), 10.0f);

	primitives[0] = new Triangle(vec3(0, 0, 5), vec3(0, 2, 5), vec3(2, 0, 5));
	//primitives[1] = new Triangle(vec3(1, 2, 2), vec3(1, 2, 2), vec3(1, 2, 2));
	primitives[2] = new Sphere(vec3(5, 0, 10), 1.0f);

	primitives[1] = new Plane(vec3(0, 0, -5), vec3(0, 0, 1));

	//primitives[1] = new Sphere(vec3(500, 0, 10), 1.0f);
	//primitives[1] = new Sphere(vec3(5, 0, 3), 1.0f);
	//primitives[0] = new Sphere(vec3(0, 0, 20), 1.0f);
	//primitives[0] = new Triangle(vec3(0, 0, 2), vec3(2, 0, 2), vec3(0, 2, 2));
	//primitives[0] = new Plane(vec3(0, 0, -5), vec3(0, 0, 1));
}

