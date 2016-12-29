#pragma once

class BVH {
public:
	glm::uint N;

	BVHNode* rootNode;
	BVHNode** pool;
	Primitive** primitives;

	//glm::uint* nodeIndices;
	glm::uint* primitiveIndices;
	glm::uint poolPtr;

	void Traverse(Ray* ray, BVHNode* node, bool isShadowRay = false);
	float IntersectPrimitives(Ray* ray, BVHNode* node);

	void ConstructBVH(Primitive** primitives);
	static AABB CalculateBounds(Primitive** primitives, int first, int count);
	float PointBoundsDist(AABB bounds, vec3 point);

	BVH(Primitive** primitives, glm::uint N)
	{
		printf("%f,%f,%f,%f,%f,%f,%f,%f,%f", primitives[0]->position.x, primitives[1]->position.x, primitives[2]->position.x, primitives[3]->position.x, primitives[4]->position.x, primitives[5]->position.x, primitives[6]->position.x, primitives[7]->position.x, primitives[10]->position.x);
		this->N = N;
		this->primitives = primitives;
		ConstructBVH(primitives);
	}
};