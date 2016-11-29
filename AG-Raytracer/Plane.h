#pragma once
#include "Primitive.h"

class Plane : public Primitive
{
public:
	vec3 normal, p;

	void CheckIntersection(Ray* ray) override;

	Plane(vec3 position, vec3 normal) : Primitive(position)
	{
		this->position = position;
		this->normal = normal;
	}
};
