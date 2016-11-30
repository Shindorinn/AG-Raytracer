#pragma once
#include "Primitive.h"

class Plane : public Primitive
{
public:
	float width, height;
	vec3 normal;

	void Init(vec3 position, vec3 normal);
	void CheckIntersection(Ray* ray) override;
	
	vec3 GetNormal(vec3 point) override;

	Plane(vec3 position, vec3 normal) : Primitive(position)
	{
		this->Init(position, normal);
	}
};
