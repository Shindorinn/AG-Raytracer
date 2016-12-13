#include "template.h"

void BVH::ConstructBVH(Primitive* primitives)
{
	// create index array
	primitiveIndices = new uint[N];
	for (int i = 0; i < N; i++) primitiveIndices[i] = i;

	// also create node indices
	nodeIndices = new int[N * 2 - 1];
	for (int i = 0; i < N * 2 - 1; i++) nodeIndices[i] = i;

	// allocate BVH root node
	pool = new BVHNode[N * 2 - 1];
	rootNode = &pool[0];
	poolPtr = 2;

	// subdivide root node
	rootNode->leftFirst = 0;
	rootNode->count = 0;
	rootNode->bounds = CalculateBounds(primitives, rootNode->leftFirst, rootNode->count);
	rootNode->Subdivide(pool, nodeIndices, poolPtr);
}

AABB BVH::CalculateBounds(Primitive* primitives, int first, int count)
{
	float minx = -INFINITY, miny = -INFINITY, minz = -INFINITY, maxx = INFINITY, maxy = INFINITY, maxz = INFINITY;

	for (int i = first; i < count; i++)
	{
		vec3 pos = primitives[i].GetPosition();

		if (pos.x < minx)
			minx = pos.x;
		else if (pos.x > maxx)
			maxx = pos.x;
		if (pos.y < miny)
			miny = pos.y;
		else if (pos.y > maxy)
			maxy = pos.y;
		if (pos.z < minz)
			minz = pos.z;
		else if (pos.z > maxz)
			maxz = pos.z;
	}
	return AABB(vec3(minx, miny, minz), vec3(maxx, maxy, maxz));
}