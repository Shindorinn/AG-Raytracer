#pragma once
#include "Ray.h"

class Primitive
{
public:
	vec3 position, normal;

	virtual void CheckIntersection(Ray* ray) = 0;

	Primitive(vec3 position);
};
