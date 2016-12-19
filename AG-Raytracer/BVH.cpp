#include "template.h"

void BVH::Traverse(Ray* ray, BVHNode* node) {
	// TODO
	if (!ray->Intersects(node->bounds)) {
		return;
	}
	if
		(node->IsLeaf())
	{
		//IntersectPrimitives();
	}
	else {
		this->Traverse(ray, pool[node->leftFirst]);
		this->Traverse(ray, pool[node->leftFirst + 1]);
	}
}

void BVH::IntersectPrimitives(Ray* ray, BVHNode* node)
{
	float smallestT = INFINITY;
	for (int i = node->leftFirst; i < node->count; i++)
	{
		if (primitives[primitiveIndices[i]]->CheckIntersection(ray) && smallestT > ray->t)
		{
			smallestT = ray->t;
			//ray->hit = pool[nodeIndices[i]];
			ray->hit = primitives[primitiveIndices[i]];
		}
	}

}


void BVH::ConstructBVH(Primitive** primitives)
{
	this->N = sizeof(primitives) / sizeof(primitives[0]);

	// create index array
	primitiveIndices = new glm::uint[N];
	for (uint i = 0; i < N; i++) primitiveIndices[i] = i;

	//// also create node indices
	//nodeIndices = new glm::uint[N * 2 - 1];
	//for (uint i = 0; i < N * 2 - 1; i++) nodeIndices[i] = i;

	// allocate BVH root node
	pool = new BVHNode*[N * 2 - 1];

	for (int i = 0; i < sizeof(this->pool) / sizeof(this->pool[0]); i++)
	{
		pool[i] = new BVHNode();
	}

	rootNode = pool[0];
	poolPtr = 2;

	// subdivide root node
	rootNode->leftFirst = 0;
	rootNode->count = 0;
	rootNode->bounds = CalculateBounds(primitives, 0, N);
	rootNode->Subdivide(pool, primitives, poolPtr);
}

AABB BVH::CalculateBounds(Primitive** primitives, int first, int count)
{
	float minx = INFINITY, miny = INFINITY, minz = INFINITY, maxx = -INFINITY, maxy = -INFINITY, maxz = -INFINITY;

	for (int i = first; i < count; i++)
	{
		vec3 pos = primitives[i]->GetPosition();

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