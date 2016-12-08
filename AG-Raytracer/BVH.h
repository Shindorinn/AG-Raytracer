#pragma once
#include "template.h";
#include "Primitive.h";
#include "BVHNode.h"

class BVH {
public:
	uint N = 0;
	BVHNode* rootNode;
	BVHNode* pool;
	
	uint* indices;
	uint poolPtr;

	void ConstructBVH(Primitive* primitives);
};

