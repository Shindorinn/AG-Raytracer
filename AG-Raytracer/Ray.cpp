#include "template.h"

Ray::Ray(vec3 origin, vec3 direction)
{
	this->origin = origin;
	this->direction = direction;
	this->t = INFINITY;
}

bool Ray::Intersects(AABB bounds) {
	return false; //TODO
}