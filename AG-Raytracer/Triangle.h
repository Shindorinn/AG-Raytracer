#pragma once
#include "Primitive.h"

class Triangle : public Primitive
{
public:
	vec3 v0, v1, v2;

	void CheckIntersection(Ray* ray) override;

	Triangle(vec3 position, vec3 v1, vec3 v2) : Primitive(position)
	{
		this->v0 = position;
		this->v1 = v1;
		this->v2 = v2;
	}
};