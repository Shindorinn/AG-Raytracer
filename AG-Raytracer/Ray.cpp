#include "template.h"

Ray::Ray(vec3 origin, vec3 direction)
{
	this->origin = origin;
	this->direction = direction;
	this->t = INFINITY;

	inv_direction = vec3(1 / direction.x, 1 / direction.y, 1 / direction.z);
	sign[0] = (inv_direction.x < 0);
	sign[1] = (inv_direction.y < 0);
	sign[2] = (inv_direction.z < 0);
}

// code from: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
//
////SLOW Intersect
//bool Ray::Intersects(AABB bounds)
//{
//	float tmin, tmax, tymin, tymax, tzmin, tzmax;
//
//	if (this->direction.x >= 0) {
//		tmin = (bounds.min.x - this->origin.x) / this->direction.x;
//		tmax = (bounds.max.x - this->origin.x) / this->direction.x;
//	}
//	else {
//		tmin = (bounds.max.x - this->origin.x) / this->direction.x;
//		tmax = (bounds.min.x - this->origin.x) / this->direction.x;
//	}
//	if (this->direction.y >= 0) {
//		tymin = (bounds.min.y - this->origin.y) / this->direction.y;
//		tymax = (bounds.max.y - this->origin.y) / this->direction.y;
//	}
//	else {
//		tymin = (bounds.max.y - this->origin.y) / this->direction.y;
//		tymax = (bounds.min.y - this->origin.y) / this->direction.y;
//	}
//
//	if ((tmin > tymax) || (tymin > tmax))
//		return false;
//
//	if (tymin > tmin)
//		tmin = tymin;
//	if (tymax < tmax)
//		tmax = tymax;
//
//	if (this->direction.z >= 0) {
//		tzmin = (bounds.min.z - this->origin.z) / this->direction.z;
//		tzmax = (bounds.max.z - this->origin.z) / this->direction.z;
//	}
//	else {
//		tzmin = (bounds.max.z - this->origin.z) / this->direction.z;
//		tzmax = (bounds.min.z - this->origin.z) / this->direction.z;
//	}
//
//	if ((tmin > tzmax) || (tzmin > tmax))
//		return false;
//	if (tzmin > tmin)
//		tmin = tzmin;
//	if (tzmax < tmax)
//		tmax = tzmax;
//
//	return true;
//}

// Optimized method
bool Ray::Intersects(AABB bounds)
{
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	int xindex, yindex, zindex;

	xindex = this->sign[0]; yindex = this->sign[1]; zindex = this->sign[2];

	if (xindex == 0)
	{
		tmin = (bounds.min.x - this->origin.x) * this->inv_direction.x;
		tmax = (bounds.max.x - this->origin.x) * this->inv_direction.x;
	}
	else
	{
		tmin = (bounds.max.x - this->origin.x) * this->inv_direction.x;
		tmax = (bounds.min.x - this->origin.x) * this->inv_direction.x;
	}

	if (yindex == 0)
	{
		tymin = (bounds.min.y - this->origin.y) * this->inv_direction.y;
		tymax = (bounds.max.y - this->origin.y) * this->inv_direction.y;
	}
	else
	{
		tymin = (bounds.max.y - this->origin.y) * this->inv_direction.y;
		tymax = (bounds.min.y - this->origin.y) * this->inv_direction.y;
	}

	if ((tmin > tymax) || (tymin > tmax))
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	if (zindex == 0)
	{
		tzmin = (bounds.min.z - this->origin.z) * this->inv_direction.z;
		tzmax = (bounds.max.z - this->origin.z) * this->inv_direction.z;
	}
	else
	{
		tzmin = (bounds.max.z - this->origin.z) * this->inv_direction.z;
		tzmax = (bounds.min.z - this->origin.z) * this->inv_direction.z;
	}

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	return true;
}