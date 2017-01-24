#pragma once

class BVH {
public:
	uint N;

	Tmpl8::BVHNode* rootNode;
	Tmpl8::BVHNode** pool;
	Tmpl8::Primitive** primitives;

	byte* whichChildFirst;
	vec4* dists;

	uint poolPtr;

	void Traverse(Tmpl8::Ray* ray, Tmpl8::BVHNode* node, bool isShadowRay = false);
	float IntersectPrimitives(Tmpl8::Ray* ray, Tmpl8::BVHNode* node);

	void ConstructBVH(Tmpl8::Primitive** primitives);
	static Tmpl8::AABB CalculateBounds(Tmpl8::Primitive** primitives, int first, int count);
	float PointBoundsDist(Tmpl8::AABB bounds, vec3 point);

	BVH(Tmpl8::Primitive** primitives, uint N)
	{
		this->N = N;
		this->primitives = primitives;
		ConstructBVH(primitives);
	}
};