#include "template.h"
#include "cone.h"

bool Cone::CheckIntersection(Ray* ray)
{
	float a = ray->direction.x*ray->direction.x + ray->direction.y*ray->direction.y + ray->direction.z*ray->direction.z;
	float b = 2 * ray->direction.x*(ray->origin.x - position.x) + 2 * ray->direction.y*(ray->origin.y - position.y) + 2 * ray->direction.z*(ray->origin.z - position.z);
	float c = position.x*position.x + position.y*position.y + position.z*position.z + ray->origin.x*ray->origin.x + ray->origin.y*ray->origin.y + ray->origin.z*ray->origin.z
		- 2 * (position.x*ray->origin.x + position.y*ray->origin.y + position.z*ray->origin.z) - r*r;

	// a*t^2 + b*t + c = 0 is solved using a discriminant = b^2-4ac
	float discriminant = b*b - 4 * a*c;

	// if discriminant is less than 0, then no hit
	// if discriminant is more than or equal to 0, then hit
	if (discriminant < 0) {
		return false;
	}

	// Intersection point components
	// t = (-b + sqrt(D))/(2*a) or t = (-b - sqrt(D))/(2*a)
	// x = x0 + tdx; y = y0 + tdy; z = z0 + tdz
	float t1 = (-b - sqrt(discriminant)) / (2 * a);
	float t2 = (-b + sqrt(discriminant)) / (2 * a);

	if (t1 < 0 && t2 < 0) {
		return false;
	}

	vec3 intersection1 = ray->origin + (t1*ray->direction);
	vec3 intersection2 = ray->origin + (t2*ray->direction);

	float distance1 = (intersection1 - ray->origin).length();
	float distance2 = (intersection2 - ray->origin).length();

	// The propper t is the one with the smalles distance, the ray passes through that one first.
	float t;
	vec3 intersection; // in case intersection point is needed.
	if (distance1 < distance2) {
		t = distance1;
		intersection = intersection1;
	}
	else {
		t = distance2;
		intersection = intersection2;
	}


	// n = (xi - xc)/r, (yi - yc)/r, (zi - zc)/r or n = (xc - xi)/r, (yc - yi)/r, (zc - zi)/r
	vec3 N = intersection - position; //normal vector pointing away from the circle
	N = normalize(N); // N.normalize();
	float function = (ray->origin.x - position.x)*(ray->origin.x - position.x) + (ray->origin.y - position.y)*(ray->origin.y - position.y) + (ray->origin.z - position.z)*(ray->origin.z - position.z) - r*r;
	if (function < 0) { //lightsource is whitin the circle
		N *= -1;
	}

	ray->t = t;
	return true;
}

vec3 Cone::GetNormal(vec3 point)
{
	return normalize(point - this->GetPosition());
}