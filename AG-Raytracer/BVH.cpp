#include "template.h"

void BVH::ConstructBVH(Primitive* primitives)
{
	// create index array
	indices = new uint[N];
	for (int i = 0; i < N; i++) indices[i] = i;

	// allocate BVH root node
	pool = new BVHNode[N * 2 - 1];
	rootNode = &pool[0];
	poolPtr = 2;

	// subdivide root node
	rootNode->leftFirst = 0;
	rootNode->count = N;
	//rootNode->bounds = CalculateBounds(primitives, rootNode->first, rootNode->count);
	rootNode->Subdivide();
}