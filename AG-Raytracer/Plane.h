#pragma once
#include "Primitive.h"

class Plane : public Primitive
{
public:
	float width, height;
	vec3 normal;

	void CheckIntersection(Ray* ray) override;

	Plane(vec3 position, float width, float height) : Primitive(position)
	{
		this->Init(position);
		this->normal = normalize(cross((vec3(width, 0, 0) + position), (vec3(0, height, 0) + position)));
	}
};
