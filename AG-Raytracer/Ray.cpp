#include "template.h"

Ray::Ray(vec3 origin, vec3 direction)
{
	this->origin = origin;
	this->direction = direction;
	this->t = INFINITY;
}

bool Ray::Intersects(AABB bounds) {
	// based on code from : https://tavianator.com/fast-branchless-raybounding-box-intersections/
	float tx1 = (bounds.min.x - this->origin.x)*(this->direction.x * -1);
	float tx2 = (bounds.max.x - this->origin.x)*(this->direction.x* -1);

	float tmin = min(tx1, tx2);
	float tmax = max(tx1, tx2);

	float ty1 = (bounds.min.y - this->origin.y)*(this->direction.y* -1);
	float ty2 = (bounds.max.y - this->origin.y)*(this->direction.y* -1);

	tmin = max(tmin, min(ty1, ty2));
	tmax = min(tmax, max(ty1, ty2));

	float tz1 = (bounds.min.z - this->origin.z)*(this->direction.z* -1);
	float tz2 = (bounds.max.z - this->origin.z)*(this->direction.z* -1);

	tmin = max(tmin, min(tz1, tz2));
	tmax = min(tmax, max(tz1, tz2));

	if (tmin > tmax)
	{
		return INFINITY;
	}

	if (tmin < 0)
	{
		return tmax;
	}

	return tmin;
	return false; //TODO
}