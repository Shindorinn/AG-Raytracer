#include "template.h"

void BVH::Traverse(Ray* ray, BVHNode* node, bool isShadowRay)
{
	//These temp/previous variables are needed, if we find multiple intersections with primitives, but we want to save the primitive with lowest t.
	float previousT = ray->t;
	Primitive* previousHit = ray->hit;

	//If we are tracing a shadow ray in the BVH, and we hit something, we can stop (improves efficiency: we early-out).
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
		{
			ray->t = previousT;
			ray->hit = previousHit;
		}
	}
	else
	{
		////We use ordered traversal here to speed up the raytracing process. We use square-dist to compare, to remove root operation :).

		//float distLeftSquared = PointBoundsDist(pool[node->leftFirst]->bounds, ray->origin);
		//float distRightSquared = PointBoundsDist(pool[node->leftFirst + 1]->bounds, ray->origin);

		//if (distLeftSquared > distRightSquared)
		//{
		//	//Right first.
		//	this->Traverse(ray, pool[node->leftFirst + 1], isShadowRay);
		//	if (ray->t * ray->t < distLeftSquared)
		//		return;
		//	this->Traverse(ray, pool[node->leftFirst], isShadowRay);
		//}
		//else
		//{
		//	//Left first.
		//	this->Traverse(ray, pool[node->leftFirst], isShadowRay);
		//	if (ray->t * ray->t < distRightSquared)
		//		return;
		//	this->Traverse(ray, pool[node->leftFirst + 1], isShadowRay);
		//}

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

float BVH::PointBoundsDist(AABB bounds, vec3 point)
{
	float dx = max(bounds.min.x - point.x, max(0.f, point.x - bounds.max.x));
	float dy = max(bounds.min.y - point.y, max(0.f, point.y - bounds.max.y));
	float dz = max(bounds.min.z - point.z, max(0.f, point.z - bounds.max.z));

	return (dx*dx + dy*dy + dz*dz);
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