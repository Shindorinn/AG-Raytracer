#pragma once

class BVH {
public:
	glm::uint N;

	BVHNode* rootNode;
	BVHNode** pool;
	Primitive** primitives;

	glm::uint poolPtr;

	void Traverse(Ray* ray, BVHNode* node, bool isShadowRay = false);
	float IntersectPrimitives(Ray* ray, BVHNode* node);

	void ConstructBVH(Primitive** primitives);
	static AABB CalculateBounds(Primitive** primitives, int first, int count);
	float PointBoundsDist(AABB bounds, vec3 point);

	BVH(Primitive** primitives, glm::uint N)
	{
		this->N = N;
		this->primitives = primitives;
		ConstructBVH(primitives);
	}
};