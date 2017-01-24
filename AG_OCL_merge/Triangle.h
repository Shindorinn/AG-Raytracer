#pragma once

class Triangle : public Tmpl8::Primitive
{
public:
	vec3 v0, v1, v2;
	vec3 normal;

	bool CheckIntersection(Tmpl8::Ray* ray) override;
	vec3 GetNormal(vec3 point) override;
	vec3 GetCentroid();
	Tmpl8::AABB* CalcAABB() override;

	Triangle(vec3 position, vec3 v1, vec3 v2) : Tmpl8::Primitive(position)
	{
		this->v0 = position;
		this->v1 = v1;
		this->v2 = v2;

		this->normal = normalize(cross(v1 - v0, v2 - v0));
		this->boundingBox = CalcAABB();
		this->centroid = GetCentroid();
	}
};