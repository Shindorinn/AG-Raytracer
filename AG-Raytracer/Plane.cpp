#include "template.h"
#include "Plane.h"

void Plane::Init(vec3 position, vec3 normal)
{
	this->normal = normalize(normal);
}

bool Plane::CheckIntersection(Ray* ray)
{
	float denominator = dot(normal, ray->direction);
	if (denominator != 0)
	{
		vec3 displacement = this->GetPosition() - ray->origin;
		ray->t = dot(displacement, normal) / denominator;
		return true;
	}
	return false;
}

vec3 Plane::GetNormal(vec3 point) {
	return this->normal;
}