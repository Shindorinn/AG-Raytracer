﻿#include "template.h"
#include "Scene.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Plane.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

Scene::Scene()
{
	camera = new Camera();

#if TUNNEL_SCENE 1
	lights[0] = new Light(vec3(0, 0, 1), vec3(100, 100, 100));
	lights[1] = new Light(vec3(-2, 0, 0), vec3(50, 50, 50));

	primitives[0] = new Plane(vec3(0, -3, 5), vec3(0, 1, 0));
	primitives[1] = new Plane(vec3(-3, 0, 5), vec3(1, 0, 0));
	primitives[2] = new Plane(vec3(3, 0, 5), vec3(-1, 0, 0));
	primitives[3] = new Plane(vec3(0, 3, 5), vec3(0, -1, 0));
	primitives[4] = new Plane(vec3(0, 0, 10), vec3(0, 0, -1));
	
	primitives[5] = new Sphere(vec3(-1, 0, 5), 1.0f);
	primitives[5]->material = Material(vec3(0, 1, 0), Material::MaterialKind::DIFFUSE);
	
	primitives[6] = new Sphere(vec3(1.5, 0, 5), 0.7f);
	primitives[6]->material = Material(vec3(1, 1, 1), Material::MaterialKind::MIRROR);

	primitives[7] = new Triangle(vec3(-1, 0, 8), vec3(-1, 2, 5), vec3(1, 0, 8));
	primitives[7]->material = Material(vec3(0, 0, 1), Material::MaterialKind::DIFFUSE);

#elif MIRROR_SCENE
	lights[0] = new Light(vec3(0, 0, 1), vec3(100, 100, 100));
	lights[1] = new Light(vec3(-5, 2, 0), vec3(50, 50, 50));

	primitives[0] = new Plane(vec3(0, 0, 10), vec3(0, 0, -1));
	primitives[0]->material = Material(vec3(0.5, 1, 0.7), Material::MaterialKind::DIFFUSE);

	primitives[1] = new Plane(vec3(0, 0, -5), vec3(0, 0, 1));
	primitives[1]->material = Material(vec3(0, 1, 0), Material::MaterialKind::DIFFUSE);

	primitives[2] = new Plane(vec3(0, 7, 0), vec3(0, -1, 0));
	primitives[2]->material = Material(vec3(0.8f, 0.8f, 0.8f), Material::MaterialKind::DIFFUSE);

	primitives[3] = new Sphere(vec3(1.5, 0, 5), 0.7f);
	primitives[3]->material = Material(vec3(1, 1, 1), Material::MaterialKind::MIRROR);

	primitives[4] = new Triangle(vec3(-1, 4, 8), vec3(-1, 6, 5), vec3(1, 4, 8));
	primitives[4]->material = Material(vec3(0, 0, 1), Material::MaterialKind::DIFFUSE);

	primitives[5] = new Sphere(vec3(-3, 3, 4), 1.0f);
	primitives[5]->material = Material(vec3(0.5f, 0.5f, 0), Material::MaterialKind::DIFFUSE);

	primitives[6] = new Sphere(vec3(3, 2, 4), 1.0f);
	primitives[6]->material = Material(vec3(0, 0.5f, 0.5f), Material::MaterialKind::DIFFUSE);

	primitives[7] = new Plane(vec3(10, 0, 0), vec3(-1, 0, 0));
	primitives[7]->material = Material(vec3(0.8f, 0.0f, 0.8f), Material::MaterialKind::DIFFUSE);

	primitives[8] = new Plane(vec3(-10, 0, 0), vec3(1, 0, 0));
	primitives[8]->material = Material(vec3(1.0f, 1.0f, 1.0f), Material::MaterialKind::MIRROR);

#elif OBJ_LOAD

	lights[0] = new Light(vec3(-2, 6, 3), vec3(200, 200, 200));

	string inputfile = "cube.obj";
	tinyobj::attrib_t attrib;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;

	string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());

	if (!err.empty()) { // `err` may contain warning message.
		std::cerr << err << std::endl;
	}

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			vec3 vertices[3];
			vec3 normal;
			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];	
				float vx = attrib.vertices[3 * idx.vertex_index + 0];
				float vy = attrib.vertices[3 * idx.vertex_index + 1];
				float vz = attrib.vertices[3 * idx.vertex_index + 2];
				float nx = attrib.normals[3 * idx.normal_index + 0];
				float ny = attrib.normals[3 * idx.normal_index + 1];
				float nz = attrib.normals[3 * idx.normal_index + 2];

				vertices[v] = vec3(vx, vy, vz);
				normal += vec3(nx, ny, nz);
			}
			index_offset += fv;

			Triangle* triangle = new Triangle(vertices[0], vertices[1], vertices[2]);
			//triangle->normal = vec3(normal.x / 3, normal.y / 3, normal.z / 3);
			primitives[f] = triangle;
		}
	}
#endif
}
