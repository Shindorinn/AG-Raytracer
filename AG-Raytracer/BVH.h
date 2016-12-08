#pragma once
#include "template.h";
#include "Primitive.h";

struct AABB{};


class BVH {
	void ConstructBVH(Primitive* primitives);
	
};

class BVHNode {
	struct BVHNode {
		AABB bounds
		int leftFirst;
		int count;
	};
	void Subdivide();
};