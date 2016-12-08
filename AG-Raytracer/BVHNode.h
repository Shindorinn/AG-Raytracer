#pragma once


class BVHNode
{
public:
	AABB bounds;
	int leftFirst;
	int count;

	void Subdivide();
	void Traverse(Ray ray);
	void Partition();
	bool IsLeaf();
	
};
