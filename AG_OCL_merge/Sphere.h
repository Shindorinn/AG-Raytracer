#pragma once

class Sphere : public Tmpl8::Primitive
{
public:
	float radius, radius2;

	bool CheckIntersection(Tmpl8::Ray* ray) override;
	vec3 GetNormal(vec3 point);
	Tmpl8::AABB* CalcAABB() override;

	Sphere(vec3 position, float radius) : Tmpl8::Primitive(position)
	{
		this->radius = radius;
		this->radius2 = radius*radius;
		this->boundingBox = CalcAABB();
		this->centroid = position;
	}
};