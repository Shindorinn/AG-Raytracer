#pragma once

class BVH {
public:
	glm::uint N = 0;

	BVHNode* rootNode;
	BVHNode* pool;

	int* nodeIndices;
	glm::uint* primitiveIndices;

	glm::uint poolPtr;

	void ConstructBVH(Primitive* primitives);
	AABB CalculateBounds(Primitive* primitives, int first, int count);

	BVH(Primitive* primitives)
	{
		this->N = sizeof(primitives) / sizeof(Primitive);
		ConstructBVH(primitives);
	}
};