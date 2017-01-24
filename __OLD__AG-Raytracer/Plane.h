#pragma once

class Plane : public Primitive
{
public:
	vec3 normal;

	bool CheckIntersection(Ray* ray) override;

	vec3 GetNormal(vec3 point) override;

	AABB* CalcAABB() override;

	Plane(vec3 position, vec3 normal) : Primitive(position)
	{
		this->normal = normal;
		this->position = position;
		this->boundingBox = CalcAABB();
	}
};