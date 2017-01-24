#include "system.h"

bool Tmpl8::Triangle::CheckIntersection(Tmpl8::Ray* ray)
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

vec3 Tmpl8::Triangle::GetNormal(vec3 point)
{
	return this->normal;
}

vec3 Tmpl8::Triangle::GetCentroid()
{
	return vec3((v0.x + v1.x + v2.x)/ 3, (v0.y + v1.y + v2.y) / 3, (v0.z + v1.z + v2.z) / 3);
}

Tmpl8::AABB* Tmpl8::Triangle::CalcAABB()
{
	float maxX = -INFINITY;
	float maxY = -INFINITY;
	float maxZ = -INFINITY;

	float minX = INFINITY;
	float minY = INFINITY;
	float minZ = INFINITY;

	if (v0.x >= maxX)
		maxX = v0.x;
	if (v1.x >= maxX)
		maxX = v1.x;
	if (v2.x >= maxX)
		maxX = v2.x;

	if (v0.y >= maxY)
		maxY = v0.y;
	if (v1.y >= maxY)
		maxY = v1.y;
	if (v2.y >= maxY)
		maxY = v2.y;

	if (v0.z >= maxZ)
		maxZ = v0.z;
	if (v1.z>= maxZ)
		maxZ = v1.z;
	if (v2.z >= maxZ)
		maxZ = v2.z;

	if (v0.x <= minX)
		minX = v0.x;
	if (v1.x <= minX)
		minX = v1.x;
	if (v2.x <= minX)
		minX = v2.x;

	if (v0.y<= minY)
		minY = v0.y;
	if (v1.y <= minY)
		minY = v1.y;
	if (v2.y <= minY)
		minY = v2.y;
		
	if (v0.z <= minZ)
		minZ = v0.z;
	if (v1.z <= minZ)
		minZ = v1.z;
	if (v2.z <= minZ)
		minZ = v2.z;

	return new Tmpl8::AABB(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));
}