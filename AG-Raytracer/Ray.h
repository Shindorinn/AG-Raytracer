#pragma once

class Primitive;
class Ray
{
public:
	vec3 origin;
	vec3 direction;	
	float t;
	Primitive* hit;

	Ray(vec3 origin, vec3 direction);
	bool Intersects(::AABB bounds);
};