#pragma once

class BVHNode
{
public:
	Tmpl8::AABB bounds;
	int leftFirst;
	int count;

	void Subdivide(Tmpl8::BVHNode** pool, Tmpl8::Primitive** primitives, uint& poolPtr);
	void UpdateBounds(Tmpl8::AABB* bounds, vec3& min, vec3& max);
	bool Partition(BVHNode** pool, Tmpl8::Primitive** primitives, uint& poolPtr);

	bool IsLeaf();

	BVHNode() {};

	BVHNode(Tmpl8::AABB bounds, int leftFirst, int count)
	{
		this->bounds = bounds;
		this->leftFirst = leftFirst;
		this->count = count;
	}
};