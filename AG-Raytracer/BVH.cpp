#include "template.h"

void BVH::Traverse(Ray* ray, BVHNode* node, bool isShadowRay)
{
	float previousT = ray->t;

	//If we are tracing a shadow ray in the BVH, and we hit something, we can stop (improves efficiency).
	if (isShadowRay && previousT != INFINITY)
		return;

	if (!ray->Intersects(node->bounds))
	{
		return;
	}
	if (node->IsLeaf())
	{
		float newT = IntersectPrimitives(ray, node);

		if (newT > previousT)
			ray->t = previousT;
	}
	else
	{
		this->Traverse(ray, pool[node->leftFirst], isShadowRay);
		this->Traverse(ray, pool[node->leftFirst + 1], isShadowRay);
	}
}

float BVH::IntersectPrimitives(Ray* ray, BVHNode* node)
{
	float smallestT = INFINITY;
	for (int i = node->leftFirst; i < node->count; i++)
	{
		//TODO: Use Indices maybe.
		if (primitives[i]->CheckIntersection(ray) && smallestT > ray->t)
		{
			smallestT = ray->t;
			ray->hit = primitives[i];
		}
	}
	if (ray->t > smallestT)
		ray->t = smallestT;
	return ray->t;
}

void BVH::ConstructBVH(Primitive** primitives)
{
	//this->N = sizeof(primitives) / sizeof(primitives[0]);

	// create index array
	primitiveIndices = new glm::uint[N];
	for (uint i = 0; i < N; i++) primitiveIndices[i] = i;

	//// also create node indices
	//nodeIndices = new glm::uint[N * 2 - 1];
	//for (uint i = 0; i < N * 2 - 1; i++) nodeIndices[i] = i;

	// allocate BVH root node
	pool = new BVHNode*[N * 2 - 1];

	for (int i = 0; i < (N * 2 - 1); i++)
	{
		pool[i] = new BVHNode();
	}

	rootNode = pool[0];
	poolPtr = 2;

	// subdivide root node
	rootNode->leftFirst = 0;
	rootNode->count = N;
	rootNode->bounds = CalculateBounds(primitives, 0, N);
	rootNode->Subdivide(pool, primitives, poolPtr, primitiveIndices);
}

AABB BVH::CalculateBounds(Primitive** primitives, int first, int last)
{
	float minx = INFINITY, miny = INFINITY, minz = INFINITY, maxx = -INFINITY, maxy = -INFINITY, maxz = -INFINITY;

	for (int i = first; i < last; i++)
	{
		AABB* bounds = primitives[i]->boundingBox;

		if (bounds->min.x < minx)
			minx = bounds->min.x;
		if (bounds->max.x > maxx)
			maxx = bounds->max.x;
		if (bounds->min.y < miny)
			miny = bounds->min.y;
		if (bounds->max.y > maxy)
			maxy = bounds->max.y;
		if (bounds->min.z < minz)
			minz = bounds->min.z;
		if (bounds->max.z > maxz)
			maxz = bounds->max.z;
	}
	return AABB(vec3(minx, miny, minz), vec3(maxx, maxy, maxz));
}