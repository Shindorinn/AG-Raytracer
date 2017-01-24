#pragma once

class Primitive;
class Ray
{
public:
	vec3 origin;
	vec3 direction;
	float t;
	Tmpl8::Primitive* hit;

	//Extra information used for faster AABB intersection test.
	vec3 inv_direction;
	int sign[3];

	Ray(vec3 origin, vec3 direction);
	bool Intersects(Tmpl8::AABB bounds);
};