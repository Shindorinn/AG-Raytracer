#include "template.h"

void BVHNode::Subdivide(BVHNode** pool, Primitive** primitives, glm::uint& poolPtr, glm::uint* primitiveIndices)
{
	if ((count - leftFirst) < 7) return;

	uint tempPoolPtr = poolPtr;

	//Get the BVHNodes temporarily, but they're not saved.
	BVHNode* left = pool[poolPtr];
	BVHNode* right = pool[poolPtr + 1];

	Partition(pool, primitives, poolPtr, primitiveIndices);

	left->Subdivide(pool, primitives, poolPtr, primitiveIndices);
	right->Subdivide(pool, primitives, poolPtr, primitiveIndices);

	this->leftFirst = tempPoolPtr;
	count = 0;
}


void BVHNode::Partition(BVHNode** pool, Primitive** primitives, glm::uint& poolPtr, glm::uint* primitiveIndices)
{
	//Use SAH (Surface Area Heuristic, described here: http://graphics.ucsd.edu/courses/cse168_s06/ucsd/heuristics.pdf 

	//C = Ct + SA(B1) / SA(B) * P1 *Ci + SA(B2) / SA(B) * P2 *Ci

	//where:

	//  Ct = node traversal cost
	//	SA() = surface area
	//	B1 = left node bounding box
	//	B2 = right node bounding box
	//	B = parent node bounding box
	//	P1 = left node primitive count
	//	P2 = left node primitive count
	//	Ci = primitive intersection cost

	//Continue to split if  c < np*ci

	//	c = estimated cost of traversing p and its children(l, r)
	//	ci = ~cost of performing one intersection test
	//	np = number of elements in parent node

	//vec3 splitAxis = vec3(1, 0, 0);
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
}

//If a node has a count of primitives, it's a leaf.
bool BVHNode::IsLeaf() {
	return count != 0;
}

//When node is a leaf, we need to use it as first. If node is an intermediate node, we need left (right = left + 1).