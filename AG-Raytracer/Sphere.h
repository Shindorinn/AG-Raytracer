#pragma once
#include "Primitive.h"

class Sphere : Primitive
{
public:
	float radius;
	vec3 center;

	void CheckIntersection(Ray ray) override;
};