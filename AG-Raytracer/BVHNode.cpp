#include "template.h"

//void BVHNode::Traverse(Ray ray, BVHNode** pool) {
//	// TODO
//	if (!ray.Intersects(bounds)) {
//		return;
//	}
//	if
//		(this->IsLeaf())
//	{
//		//IntersectPrimitives();
//	}
//	else {
//		pool[leftFirst]->Traverse(ray, pool);
//		pool[leftFirst + 1]->Traverse(ray, pool);
//	}
//}
//
//void BVHNode::IntersectPrimitives(Ray ray, BVHNode** pool)
//{
//	for (int i = leftFirst; i < count; i++)
//	{
//		if (pool[nodeindices[i]]->CheckIntersection(ray) && smallestT > ray->t)
//		{
//			smallestT = ray->t;
//			ray->hit = pool[nodeindices[i]];
//		}
//	}
//
//}

void BVHNode::Subdivide(BVHNode** pool, Primitive** primitives, glm::uint& poolPtr, glm::uint* primitiveIndices)
{
	if (count < 3) return;

	uint tempPoolPtr = poolPtr;

	/*this->leftFirst = primitives[poolPtr++];*/

	////Get the BVHNodes temporarily, but they're not saved.
	//BVHNode* left = pool[poolPtr];
	//BVHNode* right = pool[poolPtr + 1];

	////In partition, pool and nodeIndices need to be kept in sync!
	Partition(pool, primitives, poolPtr, primitiveIndices);

	////TODO: CHECK THIS    |  leftFirst + 1 will be right. Can we do this implicitly as done in the following line?  (order with partition is swapped on purpose)
	//poolPtr++;
	//this->leftFirst + 1 = nodeIndices[>poolPtr++];

	pool[poolPtr]->Subdivide(pool, primitives, poolPtr, primitiveIndices);
	pool[poolPtr + 1]->Subdivide(pool, primitives, poolPtr, primitiveIndices);
	poolPtr++;

	this->leftFirst = tempPoolPtr;
	//count = 0 hier nog ??!
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

	vec3 splitAxis = vec3(1, 0, 0);
	float width = this->bounds.max.x - this->bounds.min.x;

	vec3 newMin = vec3(this->bounds.min.x + width / 2, this->bounds.min.y, this->bounds.min.z);
	vec3 newMax = vec3(this->bounds.max.x + width / 2, this->bounds.max.y, this->bounds.max.z);

	//primitives left
	// x component less than newMin and newMax ( or whatever axis we are splitting on)

	//primitives right
	// x component more than '' ''
	uint swapLocation = poolPtr;
	for (uint i = leftFirst; i < poolPtr; i++) {
		if (primitives[primitiveIndices[i]]->centroid.x < newMin.x) {
			uint temp = primitiveIndices[i];
			primitiveIndices[i] = primitiveIndices[swapLocation];
			primitiveIndices[swapLocation] = temp;
		}
	}

}

//If a node has a count of primitives, it's a leaf.
bool BVHNode::IsLeaf() {
	return count != 0;
}

//When node is a leaf, we need to use it as first. If node is an intermediate node, we need left (right = left + 1).