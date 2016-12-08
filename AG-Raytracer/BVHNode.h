#pragma once

#include "template.h"
#include "Ray.h"
#include "BVH.h"

struct AABB {};


class BVHNode {
public:
	AABB bounds;
	int leftFirst;
	int count;

	void Subdivide();
	void Traverse(Ray ray);
	void Partition();
	bool IsLeaf();
};