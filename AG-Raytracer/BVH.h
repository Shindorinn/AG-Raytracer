#pragma once

class BVH {
public:
	glm::uint N;

	BVHNode* rootNode;
	BVHNode** pool;
	Primitive** primitives;

	//glm::uint* nodeIndices;
	glm::uint* primitiveIndices;
	glm::uint poolPtr;
	
	void Traverse(Ray* ray, BVHNode* node);
	void IntersectPrimitives(Ray* ray, BVHNode* node);

	void ConstructBVH(Primitive** primitives);
	AABB CalculateBounds(Primitive** primitives, int first, int count);

	BVH(Primitive** primitives)
	{
		this->N = sizeof(primitives) / sizeof(Primitive);
		this->primitives = primitives;
		ConstructBVH(primitives);
	}
};