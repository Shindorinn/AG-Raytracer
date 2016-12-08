#pragma once
#include "BVH.h"

class Ray
{
public:
	vec3 origin;
	vec3 direction;	
	float t;
	Ray(vec3 origin, vec3 direction);
	bool Intersects(AABB bounds);
};