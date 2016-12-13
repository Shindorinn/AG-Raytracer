#pragma once

class BVHNode
{
public:
	AABB bounds;
	int leftFirst;
	int count;

	void Subdivide(BVHNode* pool, int* nodeIndices, glm::uint poolPtr);
	void Traverse(Ray ray);
	void Partition(BVHNode* pool, int* nodeIndices, glm::uint poolPtr);
	bool IsLeaf();

	BVHNode() {};

	BVHNode(AABB bounds, int leftFirst, int count)
	{
		this->bounds = bounds;
		this->leftFirst = leftFirst;
		this->count = count;
	}
};