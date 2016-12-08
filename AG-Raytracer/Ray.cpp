#include "template.h"
#include "Ray.h"

Ray::Ray(vec3 origin, vec3 direction)
{
	this->origin = origin;
	this->direction = direction;
	this->t = INFINITY;
}

bool Ray::Intersects(AABB bounds) {
	return false; //TODO
}