#include "template.h"

void BVHNode::Subdivide(BVHNode* pool, glm::uint poolPtr)
{
	//if (count < 3) return;

	//this->leftFirst = pool[poolPtr++];
	//this->leftFirst + 1 = pool[>poolPtr++];

	//Partition();

	//this->leftFirst->Subdivide();
	//this->leftFirst+1->Subdivide();

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

void BVHNode::Partition() {

}

bool BVHNode::IsLeaf() {
	return count == 0;
}


//When count != 0, we need to use it as first.