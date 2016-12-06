#pragma once
#include "Primitive.h"


class Cone : public Primitive
{
public:
    float r;

	bool CheckIntersection(Ray* ray) override;
	vec3 GetNormal(vec3 point) override;
	
	Cone(vec3 position, float r) : Primitive(position)
	{
		this->r = r;
	}
};