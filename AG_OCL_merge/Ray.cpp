#include "system.h"

Tmpl8::Ray::Ray(vec3 origin, vec3 direction)
{
	this->origin = origin;
	this->direction = direction;
	this->t = INFINITY;

	inv_direction = vec3(1 / direction.x, 1 / direction.y, 1 / direction.z);
	sign[0] = (inv_direction.x < 0);
	sign[1] = (inv_direction.y < 0);
	sign[2] = (inv_direction.z < 0);
}

bool Tmpl8::Ray::Intersects(Tmpl8::AABB bounds)
{
	float tx1 = (bounds.min.x - this->origin.x) * this->inv_direction.x;
	float tx2 = (bounds.max.x - this->origin.x) * this->inv_direction.x;

	float tmin = min(tx1, tx2);
	float tmax = max(tx1, tx2);

	float ty1 = (bounds.min.y - this->origin.y) * this->inv_direction.y;
	float ty2 = (bounds.max.y - this->origin.y) * this->inv_direction.y;

	tmin = max(tmin, min(ty1, ty2));
	tmax = min(tmax, max(ty1, ty2));

	float tz1 = (bounds.min.z - this->origin.z) * this->inv_direction.z;
	float tz2 = (bounds.max.z - this->origin.z) * this->inv_direction.z;

	tmin = max(tmin, min(tz1, tz2));
	tmax = min(tmax, max(tz1, tz2));

	return tmax >= tmin && tmax >= 0;
}