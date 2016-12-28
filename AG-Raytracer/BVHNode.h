#pragma once

class BVHNode
{
public:
	AABB bounds;
	int leftFirst;
	int count;

	//void Traverse(Ray ray, BVHNode** pool);
	//void BVHNode::IntersectPrimitives(Ray ray, BVHNode** pool);

	void Subdivide(BVHNode** pool, Primitive** primitives, glm::uint& poolPtr, glm::uint* primitiveIndices);
	void UpdateBounds(AABB* bounds, vec3& min, vec3& max);
	bool Partition(BVHNode** pool, Primitive** primitives, glm::uint& poolPtr, glm::uint* primitiveIndices);

	bool IsLeaf();


	BVHNode() {};

	BVHNode(AABB bounds, int leftFirst, int count)
	{
		this->bounds = bounds;
		this->leftFirst = leftFirst;
		this->count = count;
	}
};