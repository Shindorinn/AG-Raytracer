#pragma once

class Plane : public Tmpl8::Primitive
{
public:
	vec3 normal;

	bool CheckIntersection(Tmpl8::Ray* ray) override;

	vec3 GetNormal(vec3 point) override;

	Tmpl8::AABB* CalcAABB() override;

	Plane(vec3 position, vec3 normal) : Tmpl8::Primitive(position)
	{
		this->normal = normal;
		this->position = position;
		this->boundingBox = CalcAABB();
	}
};