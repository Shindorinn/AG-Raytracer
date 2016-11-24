#pragma once
#include "Primitive.h"

class Sphere : public Primitive
{
public:
	float radius;

	void CheckIntersection(Ray* ray) override;

	Sphere(vec3 position, float radius) : Primitive(position)
	{		
		this->radius = radius;
	}
};