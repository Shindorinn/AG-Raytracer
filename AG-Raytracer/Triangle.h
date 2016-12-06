#pragma once
#include "Primitive.h"

class Triangle : public Primitive
{
public:
	vec3 v0, v1, v2;
	vec3 normal;

	bool CheckIntersection(Ray* ray) override;
	vec3 GetNormal(vec3 point) override;

	Triangle(vec3 position, vec3 v1, vec3 v2) : Primitive(position)
	{
		this->v0 = position;
		this->v1 = v1;
		this->v2 = v2;

		this->normal = normalize(cross(v1 - v0, v2 - v0));
	}
};