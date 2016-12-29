#include "template.h"

#define ORDERED_TRAVERSAL1 0
#define ORDERED_TRAVERSAL3 0
#define NAIVE_TRAVERSAL 1

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
#if ORDERED_TRAVERSAL1
		//Ordered Traversal Option 1
		//We use ordered traversal here to speed up the raytracing process. We use square-dist to compare, to remove root operation :).

		float distLeftSquared = PointBoundsDist(pool[node->leftFirst]->bounds, ray->origin);
		float distRightSquared = PointBoundsDist(pool[node->leftFirst + 1]->bounds, ray->origin);

		if (distLeftSquared > distRightSquared)
		{
			//Right first.
			this->Traverse(ray, pool[node->leftFirst + 1], isShadowRay);
			if (ray->t * ray->t < distLeftSquared)
				return;
			this->Traverse(ray, pool[node->leftFirst], isShadowRay);
		}
		else
		{
			//Left first.
			this->Traverse(ray, pool[node->leftFirst], isShadowRay);
			if (ray->t * ray->t < distRightSquared)
				return;
			this->Traverse(ray, pool[node->leftFirst + 1], isShadowRay);
		}
#endif

#if ORDERED_TRAVERSAL3
		//Ordered Traversal Option 3

		if (whichChildFirst[node->leftFirst] == 1)
		{
			//Do left first
			this->Traverse(ray, pool[node->leftFirst], isShadowRay);

			if (ray->t < dists[node->leftFirst][dists[node->leftFirst].w])
				return;

			this->Traverse(ray, pool[node->leftFirst + 1], isShadowRay);
		}
		else if (whichChildFirst[node->leftFirst] == 2)
		{
			//Do right first
			this->Traverse(ray, pool[node->leftFirst + 1], isShadowRay);

			if (ray->t < dists[node->leftFirst][dists[node->leftFirst].w])
				return;

			this->Traverse(ray, pool[node->leftFirst], isShadowRay);
		}

		//If it's not already computed, compute it.
		else
		{
			vec3 leftCentroid = pool[node->leftFirst]->bounds.centroid;
			vec3 rightCentroid = pool[node->leftFirst + 1]->bounds.centroid;

			float xdist = abs(rightCentroid.x - leftCentroid.x);
			float ydist = abs(rightCentroid.y - leftCentroid.y);
			float zdist = abs(rightCentroid.z - leftCentroid.z);
			vec3 dist = vec3(xdist, ydist, zdist);

			int dim = 0;
			if (ydist >= xdist && ydist >= zdist)
				dim = 1;
			if (zdist >= xdist && zdist >= ydist)
				dim = 2;

			//If this is true, the sign is +, else -.
			bool sign = ray->direction[dim] > 0;

			dists[node->leftFirst] = vec4(dist, dim);

			if (sign)
			{
				//We know that left is closer, so do left first.
				if (leftCentroid[dim] < rightCentroid[dim])
				{
					whichChildFirst[node->leftFirst] = 1;
					//Left first.
					this->Traverse(ray, pool[node->leftFirst], isShadowRay);
					if (ray->t < dist[dim])
						return;
					this->Traverse(ray, pool[node->leftFirst + 1], isShadowRay);
				}
				else
				{
					whichChildFirst[node->leftFirst] = 2;
					//Right first.
					this->Traverse(ray, pool[node->leftFirst + 1], isShadowRay);
					if (ray->t < dist[dim])
						return;
					this->Traverse(ray, pool[node->leftFirst], isShadowRay);
				}


			}
			else
			{
				//We know that left is closer, so do left first.
				if (leftCentroid[dim] > rightCentroid[dim])
				{
					whichChildFirst[node->leftFirst] = 1;
					//Left first.
					this->Traverse(ray, pool[node->leftFirst], isShadowRay);
					if (ray->t < dist[dim])
						return;
					this->Traverse(ray, pool[node->leftFirst + 1], isShadowRay);
				}
				else
				{
					whichChildFirst[node->leftFirst] = 2;
					//Right first.
					this->Traverse(ray, pool[node->leftFirst + 1], isShadowRay);
					if (ray->t < dist[dim])
						return;
					this->Traverse(ray, pool[node->leftFirst], isShadowRay);
				}
			}
		}
#endif

#if NAIVE_TRAVERSAL
		//Naive Traversal
		this->Traverse(ray, pool[node->leftFirst], isShadowRay);
		this->Traverse(ray, pool[node->leftFirst + 1], isShadowRay);
#endif
	}
}

float BVH::IntersectPrimitives(Ray* ray, BVHNode* node)
{
	float smallestT = INFINITY;
	for (int i = node->leftFirst; i < node->count; i++)
	{
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
	// allocate BVH root node
	pool = new BVHNode*[N * 2 - 1];

	//whichChildFirst is used for ordered traversal precomputing, as well as dists.
	whichChildFirst = new byte[N * 2 - 1];
	dists = new vec4[N * 2 - 1];

	for (int i = 0; i < (N * 2 - 1); i++)
	{
		pool[i] = new BVHNode();
		whichChildFirst[i] = 0;
		dists[i] = vec4(0);
	}

	rootNode = pool[0];
	poolPtr = 2;

	// subdivide root node
	rootNode->leftFirst = 0;
	rootNode->count = N;
	rootNode->bounds = CalculateBounds(primitives, 0, N);
	rootNode->Subdivide(pool, primitives, poolPtr);
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