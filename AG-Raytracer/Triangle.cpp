#include "template.h"
#include "Triangle.h"

bool Triangle::CheckIntersection(Ray* ray)
{
#define Epsilon 0.000001
	// Vectors from v0 to v1/v2 (edges)
	vec3 e1, e2;

	vec3 p, q, t;
	float det, invDet, u, v;

	//Find vectors for two edges sharing vertex/point v0
	e1 = v1 - v0;
	e2 = v2 - v0;

	// calculating determinant 
	p = cross(ray->direction, e2);

	//Calculate determinat
	det = dot(e1, p);

	//if determinant is near zero, ray lies in plane of triangle otherwise not
	if (det > -Epsilon && det < Epsilon) { return false; }
	invDet = 1.0f / det;

	//calculate distance from v0 to ray origin
	t = ray->origin - v0;

	//Calculate u parameter
	u = dot(t, p) * invDet;

	//Check for ray hit
	if (u < 0 || u > 1) { return false; }

	//Prepare to test v parameter
	q = cross(t, e1);

	//Calculate v parameter
	v = dot(ray->direction, q) * invDet;

	//Check for ray hit
	if (v < 0 || u + v > 1) { return false; }

	float tValue = dot(e2, q) * invDet;
	if (tValue > Epsilon)
	{
		//ray does intersect
		ray->t = tValue;
		return true;
	}

	// No hit at all
	return false;
}

vec3 Triangle::GetNormal(vec3 point)
{
	return this->normal;
} 