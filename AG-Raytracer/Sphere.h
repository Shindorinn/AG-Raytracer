#pragma once
#include "Primitive.h"

class Sphere : public Primitive
{
public:
	float radius, radius2;

	bool CheckIntersection(Ray* ray) override;
	vec3 GetNormal(vec3 point);

	Sphere(vec3 position, float radius) : Primitive(position)
	{
		this->radius = radius;
		this->radius2 = radius*radius;
	}
};