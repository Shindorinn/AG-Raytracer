#include "template.h"

void BVHNode::Subdivide(BVHNode* pool, int* nodeIndices, glm::uint poolPtr)
{
	if (count < 3) return;

	this->leftFirst = nodeIndices[poolPtr++];

	//In partition, pool and nodeIndices need to be kept in sync!
	Partition(pool, nodeIndices, poolPtr);

	//TODO: CHECK THIS    |  leftFirst + 1 will be right. Can we do this implicitly as done in the following line?  (order with partition is swapped on purpose)
	poolPtr++;
	//this->leftFirst + 1 = nodeIndices[>poolPtr++];

	pool[leftFirst].Subdivide(pool, nodeIndices, poolPtr);
	pool[leftFirst + 1].Subdivide(pool, nodeIndices, poolPtr);
}

void BVHNode::Traverse(Ray ray) {
	// TODO
	if (!ray.Intersects(bounds)) {
		return;
	}
	if
		(this->IsLeaf())
	{
		//IntersectPrimitives();
	}
	else {
		//bvh->pool[left].Traverse(ray);
		//bvh->pool[left + 1].Traverse(ray);
	}
}

void BVHNode::Partition(BVHNode* pool, int* nodeIndices, glm::uint poolPtr)
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
}

//If a node has a count of primitives, it's a leaf.
bool BVHNode::IsLeaf() {
	return count != 0;
}

//When node is a leaf, we need to use it as first. If node is an intermediate node, we need left (right = left + 1).