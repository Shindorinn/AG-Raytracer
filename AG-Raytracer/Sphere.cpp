#include "template.h"
#include "Sphere.h"

//This only works for rays that start outside the sphere.
//TODO: use the other (slower) interesection for glass/water-like objects (because refraction causes rays to start inside the sphere).
void Sphere::CheckIntersection(Ray* ray)
{
	vec3 c = this->GetPosition() - ray->origin;
	float t = dot(c, ray->direction);
	vec3 q = c - t * ray->direction;
	float p2 = dot(q, q);

	float r2 = radius * radius;

	if (p2 > r2) return; // r2 = r * r

	t -= glm::sqrt(r2 - p2);
	if ((t < ray->t) && (t > 0)) ray->t = t;
	// or: ray.t = min( ray.t, max( 0, t ) );
}

vec3 Sphere::GetNormal(vec3 point)
{
	return normalize(this->GetPosition() - point);
}