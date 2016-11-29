#include "template.h"
#include "Plane.h"

void Plane::CheckIntersection(Ray* ray)
{
	float denominator = dot(normal, ray->direction);
	if (denominator != 0)
	{
		vec3 displacement = position - ray->origin;
		ray->t = dot(displacement, normal) / denominator;
	}
	return;
}