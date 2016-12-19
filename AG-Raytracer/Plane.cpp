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

AABB* Plane::CalcAABB() {
	// By definition a bounding box for a plane is infinite as soon as it is slightly angled
	// For sake of ease, a bounding box for a plane will always be infinitely large
	return new AABB(vec3(-INFINITE, -INFINITE, -INFINITE), vec3(INFINITE, INFINITE, INFINITE));
}