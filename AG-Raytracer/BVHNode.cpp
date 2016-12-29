#include "template.h"

#define USESAH 1
#define BINNING 1

void BVHNode::Subdivide(BVHNode** pool, Primitive** primitives, glm::uint& poolPtr)
{
#if !USESAH
	if ((count - leftFirst) < 5) return;
#endif

	uint tempPoolPtr = poolPtr;

	//Get the BVHNodes temporarily, but they're not saved.
	BVHNode* left = pool[poolPtr];
	BVHNode* right = pool[poolPtr + 1];

	if (!Partition(pool, primitives, poolPtr))
		return;

	left->Subdivide(pool, primitives, poolPtr);
	right->Subdivide(pool, primitives, poolPtr);

	this->leftFirst = tempPoolPtr;
	count = 0;
}


bool BVHNode::Partition(BVHNode** pool, Primitive** primitives, glm::uint& poolPtr)
{
#if USESAH 1
	float parentCost = this->bounds.GetVolume() * (count - leftFirst);

	float lowestCost = INFINITE;
	int bestDimension;
	float bestCoord;
#if !BINNING
	//Iterate over all possible splits, calculate A * R
	for (int i = leftFirst; i < count; i++)
	{
		for (int dimension = 0; dimension < 3; dimension++)
		{
			float splitCoord = primitives[i]->centroid[dimension];

#else
	float xwidth = this->bounds.max.x - this->bounds.min.x;
	float ywidth = this->bounds.max.y - this->bounds.min.y;
	float zwidth = this->bounds.max.z - this->bounds.min.z;
	vec3 widths = vec3(xwidth, ywidth, zwidth);
	for (int i = 1; i < 7; i++)
	{
		for (int dimension = 0; dimension < 3; dimension++)
		{
			float splitCoord = this->bounds.min[dimension] + widths[dimension] * (i / 7.0f);
#endif

			int leftCounter = 0, rightCounter = 0;

			vec3 leftMin = vec3(INFINITY), leftMax = vec3(-INFINITY), rightMin = vec3(INFINITY), rightMax = vec3(-INFINITY);

			//Bounds are calculated when objects are added. This is faster than doing it afterwards (no double looping over objects).
			for (uint32_t i = leftFirst; i < count; i++)
			{
				if (primitives[i]->centroid[dimension] < splitCoord)
				{
					AABB* bounds = primitives[i]->boundingBox;

					UpdateBounds(bounds, leftMin, leftMax);

					leftCounter++;
				}
				else
				{
					AABB* bounds = primitives[i]->boundingBox;

					UpdateBounds(bounds, rightMin, rightMax);

					rightCounter++;
				}
			}

			//Calculate left- and rightCost, save this value (and corresponding dimension/coord) if it's better than the previous best solution.
			float splitCost = AABB(leftMin, leftMax).GetVolume() * leftCounter + AABB(rightMin, rightMax).GetVolume() * rightCounter;

			if (splitCost < lowestCost)
			{
				lowestCost = splitCost;
				bestDimension = dimension;
				bestCoord = splitCoord;
			}
		}

	}

	if (lowestCost >= parentCost)
		//We know that we don't need to split now; the node has become a child node.
		return false;

	uint32_t mid = leftFirst;
	for (uint32_t i = leftFirst; i < count; i++)
	{
		if (primitives[i]->centroid[bestDimension] < bestCoord)
		{
			std::swap(primitives[i], primitives[mid]);
			mid++;
		}
	}

	//Left node.
	pool[poolPtr]->leftFirst = leftFirst;
	pool[poolPtr]->count = mid;
	pool[poolPtr]->bounds = BVH::CalculateBounds(primitives, pool[poolPtr]->leftFirst, pool[poolPtr]->count);

	poolPtr++;

	//Right node.
	pool[poolPtr]->leftFirst = mid;
	pool[poolPtr]->count = count;
	pool[poolPtr]->bounds = BVH::CalculateBounds(primitives, pool[poolPtr]->leftFirst, pool[poolPtr]->count);

	poolPtr++;

	return true;
#else
	//Median split
	float xwidth = this->bounds.max.x - this->bounds.min.x;
	float ywidth = this->bounds.max.y - this->bounds.min.y;
	float zwidth = this->bounds.max.z - this->bounds.min.z;

	int dimension;

	float biggest = max(xwidth, max(ywidth, zwidth));
	if (biggest == xwidth)
		dimension = 0;
	else if (biggest == ywidth)
		dimension = 1;
	else
		dimension = 2;

	float splitCoord = 0.5f*(bounds.max[dimension] + bounds.min[dimension]);

	uint32_t mid = leftFirst;
	for (uint32_t i = leftFirst; i < count; i++)
	{
		if (primitives[i]->centroid[dimension] < splitCoord)
		{
			std::swap(primitives[i], primitives[mid]);
			mid++;
		}
	}

	//Left node.
	pool[poolPtr]->leftFirst = leftFirst;
	pool[poolPtr]->count = mid;
	pool[poolPtr]->bounds = BVH::CalculateBounds(primitives, pool[poolPtr]->leftFirst, pool[poolPtr]->count);

	poolPtr++;

	//Right node.
	pool[poolPtr]->leftFirst = mid;
	pool[poolPtr]->count = count;
	pool[poolPtr]->bounds = BVH::CalculateBounds(primitives, pool[poolPtr]->leftFirst, pool[poolPtr]->count);

	poolPtr++;

	return true;
#endif
		}

void BVHNode::UpdateBounds(AABB* bounds, vec3& min, vec3& max)
{
	if (bounds->min.x < min.x)
		min.x = bounds->min.x;
	if (bounds->max.x > max.x)
		max.x = bounds->max.x;
	if (bounds->min.y < min.y)
		min.y = bounds->min.y;
	if (bounds->max.y > max.y)
		max.y = bounds->max.y;
	if (bounds->min.z < min.z)
		min.z = bounds->min.z;
	if (bounds->max.z > max.z)
		max.z = bounds->max.z;
}


//If a node has a count of primitives, it's a leaf.
bool BVHNode::IsLeaf() {
	return count != 0;
}

//When node is a leaf, we need to use it as first. If node is an intermediate node, we need left (right = left + 1).