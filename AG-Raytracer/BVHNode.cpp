#include "BVH.h"
BVH* bvh;

void BVHNode::Subdivide()
{
	//TODO
	if (count < 3) return;
	//this->left = bvh->pool[bvh->poolPtr++];
	//this->right = bvh->pool[bvh->poolPtr++];
	Partition();
	//this->left->Subdivide();
	//this->right->Subdivide();
	this->IsLeaf = false;
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
}void BVHNode::Partition() {}bool BVHNode::IsLeaf() {	return false;}