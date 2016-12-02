#include "template.h"
#include "Sphere.h"

//This only works for rays that start outside the sphere.
//TODO: use the other (slower) intersection for glass/water-like objects (because refraction causes rays to start inside the sphere).
bool Sphere::CheckIntersection(Ray* ray)
{
	vec3 c = this->GetPosition() - ray->origin;
	float t = dot(c, ray->direction);
	vec3 q = c - t * ray->direction;
	float p2 = dot(q, q);

	if (p2 > radius2) return false; // r2 = r * r

	//printf("Sphere : Intersection found!");

	t -= glm::sqrt(radius2 - p2);
	if ((t < ray->t) && (t > 0)) ray->t = t;
	// or: ray.t = min( ray.t, max( 0, t ) );

	return true;
}

vec3 Sphere::GetNormal(vec3 point)
{
	return normalize(point - this->GetPosition());
}