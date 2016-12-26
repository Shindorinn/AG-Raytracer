#include "template.h"

Ray::Ray(vec3 origin, vec3 direction)
{
	this->origin = origin;
	this->direction = direction;
	this->t = INFINITY;
}

// code from: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection

bool Ray::Intersects(AABB bounds)
{
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	if (this->direction.x >= 0) {
		tmin = (bounds.min.x - this->origin.x) / this->direction.x;
		tmax = (bounds.max.x - this->origin.x) / this->direction.x;
	}
	else {
		tmin = (bounds.max.x - this->origin.x) / this->direction.x;
		tmax = (bounds.min.x - this->origin.x) / this->direction.x;
	}
	if (this->direction.y >= 0) {
		tymin = (bounds.min.y - this->origin.y) / this->direction.y;
		tymax = (bounds.max.y - this->origin.y) / this->direction.y;
	}
	else {
		tymin = (bounds.max.y - this->origin.y) / this->direction.y;
		tymax = (bounds.min.y - this->origin.y) / this->direction.y;
	}

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	if (this->direction.z >= 0) {
		tzmin = (bounds.min.z - this->origin.z) / this->direction.z;
		tzmax = (bounds.max.z - this->origin.z) / this->direction.z;
	}
	else {
		tzmin = (bounds.max.z - this->origin.z) / this->direction.z;
		tzmax = (bounds.min.z - this->origin.z) / this->direction.z;
	}

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	return true;
}
//
//bool Ray::Intersects(AABB bounds) {
//	// based on code from : https://tavianator.com/fast-branchless-raybounding-box-intersections/
//	float tx1 = (bounds.min.x - this->origin.x)*(this->direction.x * -1);
//	float tx2 = (bounds.max.x - this->origin.x)*(this->direction.x * -1);
//
//	float tmin = min(tx1, tx2);
//	float tmax = max(tx1, tx2);
//
//	float ty1 = (bounds.min.y - this->origin.y)*(this->direction.y * -1);
//	float ty2 = (bounds.max.y - this->origin.y)*(this->direction.y * -1);
//
//	tmin = max(tmin, min(ty1, ty2));
//	tmax = min(tmax, max(ty1, ty2));
//
//	float tz1 = (bounds.min.z - this->origin.z)*(this->direction.z * -1);
//	float tz2 = (bounds.max.z - this->origin.z)*(this->direction.z * -1);
//
//	tmin = max(tmin, min(tz1, tz2));
//	tmax = min(tmax, max(tz1, tz2));
//
//	if (tmin > tmax)
//	{
//		return false;
//	}
//
//	//if (tmin < 0)
//	//{
//	//	return tmax;
//	//}
//	//return tmin;
//
//	return true; //TODO : test
//}