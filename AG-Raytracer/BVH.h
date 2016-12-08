#pragma once

class Primitive;
class BVHNode;

class BVH {
public:
	glm::uint N = 0;
	BVHNode* rootNode;
	BVHNode* pool;

	glm::uint* indices;
	glm::uint poolPtr;

	void ConstructBVH(Primitive* primitives);
};