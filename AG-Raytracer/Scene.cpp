#include "template.h"
#include "Scene.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Plane.h"

Scene::Scene()
{
	camera = new Camera();
	//lights[0] = new Light(vec3(2, 5, 2), 10.0f);
	lights[0] = new Light(vec3(2, 0, 1), vec3(100, 100, 100));
	//lights[1] = new Light(vec3(1, 5, 0), vec3(50, 50, 50));


	//primitives[1] = new Triangle(vec3(0, 0, 5), vec3(0, 2, 5), vec3(2, 0, 5));
	//primitives[1] = new Triangle(vec3(1, 2, 2), vec3(1, 2, 2), vec3(1, 2, 2));

	//primitives[0] = new Sphere(vec3(0, 0, 2), 1.0f);
	//primitives[1] = new Sphere(vec3(0, 5, 0), 1.0f);


	//primitives[1] = new Sphere(vec3(-2, 0, 4), 1.2f);
	//primitives[1]->material = Material(vec3(1, 1, 1), Material::MaterialKind::DIFFUSE);


	//primitives[2] = new Sphere(vec3(0, 0, 5), 1.0f);
	//primitives[3] = new Sphere(vec3(0, 0, -5), 1.0f);
	//primitives[4] = new Sphere(vec3(5, 0, 0), 1.0f);
	//primitives[5] = new Sphere(vec3(-5, 0, 0), 1.0f);

	primitives[0] = new Plane(vec3(0, -3, 5), vec3(0, 1, 0));
	primitives[1] = new Plane(vec3(-3, 0, 5), vec3(1, 0, 0));
	primitives[2] = new Plane(vec3(3, 0, 5), vec3(-1, 0, 0));
	primitives[3] = new Plane(vec3(0, 3, 5), vec3(0, -1, 0));


	primitives[4] = new Sphere(vec3(-1, 0, 5), 1.0f);
	primitives[4]->material = Material(vec3(0, 1, 0), Material::MaterialKind::DIFFUSE);

	//primitives[4] = new Sphere(vec3(0.5, 0, 5), 1.0f);
	//primitives[4]->material = Material(vec3(0, 1, 0), Material::MaterialKind::DIFFUSE);

	//primitives[5] = new Sphere(vec3(-2, 0, 5), 0.7f);
	//primitives[5]->material = Material(vec3(1, 1, 0), Material::MaterialKind::DIFFUSE);

	//primitives[1] = new Sphere(vec3(500, 0, 10), 1.0f);
	//primitives[1] = new Sphere(vec3(5, 0, 3), 1.0f);
	//primitives[0] = new Sphere(vec3(0, 0, 20), 1.0f);
	//primitives[0] = new Triangle(vec3(0, 0, 2), vec3(2, 0, 2), vec3(0, 2, 2));

	//primitives[0] = new Triangle(vec3(0, 0, 5), vec3(0, 2, 5), vec3(2, 0, 5));
	//primitives[1] = new Triangle(vec3(1, 2, 2), vec3(1, 2, 2), vec3(1, 2, 2));
	//primitives[2] = new Sphere(vec3(5, 0, 10), 1.0f);

	//primitives[1] = new Sphere(vec3(500, 0, 10), 1.0f);
	//primitives[1] = new Sphere(vec3(5, 0, 3), 1.0f);
	//primitives[0] = new Sphere(vec3(2, 0, 10), 1.0f);
	//primitives[1] = new Sphere(vec3(-2, 8, 10), 1.5f);
	//primitives[0] = new Triangle(vec3(-1, 0, 1), vec3(0, 1, 7), vec3(1, 1, 7));
	//primitives[1] = new Triangle(vec3(2, 1, 4), vec3(-1, 0, 4), vec3(-1, 1, 5));
	//primitives[0]->material = Material(vec3(0, 1, 0), Material::MaterialKind::DIFFUSE);
	//primitives[4] = new Triangle(vec3(0, 0, 5), vec3(2, 0, 2), vec3(0, 2, 2));
	//primitives[0] = new Plane(vec3(0, 0, 2), vec3(0, 1, 0));
}