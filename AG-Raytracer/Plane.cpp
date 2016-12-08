#include "template.h"

bool Plane::CheckIntersection(Ray* ray)
{
	float denominator = dot(normal, ray->direction);
	if (abs(denominator) > 0.0001f)
	{
		float t = dot((this->GetPosition() - ray->origin), normal) / denominator;
		if (t >= 0.0001f)
		{
			ray->t = t;
			return true;
		}
	}
	return false;
}

vec3 Plane::GetNormal(vec3 point) {
	return this->normal;
}