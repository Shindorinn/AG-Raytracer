#include "template.h"

#define USESAH 1

void BVHNode::Subdivide(BVHNode** pool, Primitive** primitives, glm::uint& poolPtr, glm::uint* primitiveIndices)
{
#if !USESAH
	if ((count - leftFirst) < 5) return;
#endif

	uint tempPoolPtr = poolPtr;

	//Get the BVHNodes temporarily, but they're not saved.
	BVHNode* left = pool[poolPtr];
	BVHNode* right = pool[poolPtr + 1];

	if (!Partition(pool, primitives, poolPtr, primitiveIndices))
		return;

	left->Subdivide(pool, primitives, poolPtr, primitiveIndices);
	right->Subdivide(pool, primitives, poolPtr, primitiveIndices);

	this->leftFirst = tempPoolPtr;
	count = 0;
}


bool BVHNode::Partition(BVHNode** pool, Primitive** primitives, glm::uint& poolPtr, glm::uint* primitiveIndices)
{
#if USESAH 1
	float parentCost = this->bounds.GetVolume() * (count - leftFirst);

	float lowestCost = INFINITE;
	int bestDimension;
	float bestCoord;

	//Iterate over all possible splits, calculate A * R
	for (int i = leftFirst; i < count; i++)
	{
		for (int dimension = 0; dimension < 3; dimension++)
		{
			float splitCoord = primitives[i]->centroid[dimension];

			//Arrays to store the primitives for the current split.
			Primitive** left = new Primitive*[count - leftFirst];
			Primitive** right = new Primitive*[count - leftFirst];

			int leftCounter = 0, rightCounter = 0;

			for (uint32_t i = leftFirst; i < count; i++)
			{
				if (primitives[i]->centroid[dimension] < splitCoord)
				{
					left[leftCounter] = primitives[i];
					leftCounter++;
				}
				else
				{
					right[rightCounter] = primitives[i];
					rightCounter++;
				}
			}

			AABB leftBounds = BVH::CalculateBounds(left, 0, leftCounter);
			AABB rightBounds = BVH::CalculateBounds(right, 0, rightCounter);

			//Calculate left- and rightCost, save this value (and corresponding dimension/coord) if it's better than the previous best solution.
			float splitCost = leftBounds.GetVolume() * leftCounter + rightBounds.GetVolume() * rightCounter;

			if (splitCost < lowestCost)
			{
				lowestCost = splitCost;
				bestDimension = dimension;
				bestCoord = splitCoord;
			}

			delete left;
			delete right;
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

//If a node has a count of primitives, it's a leaf.
bool BVHNode::IsLeaf() {
	return count != 0;
}

//When node is a leaf, we need to use it as first. If node is an intermediate node, we need left (right = left + 1).