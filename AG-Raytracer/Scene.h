#pragma once

#define TUNNEL_SCENE 1
#define f16_LOAD 0

class Scene
{
public:

#if TUNNEL_SCENE
	Primitive* primitives[14];
	Light* lights[2];
	Entity* entities[16];

#elif f16_LOAD
	Primitive* primitives[4058 + 10 - 2];
	Light* lights[2];
	Entity* entities[4060 + 10 - 2];

#endif

	Camera* camera;
	AABB* sceneBounds;
	BVH* bvh;
	Scene();

	int lightCount;
	int primCount;

	AABB* CalculateSceneBounds();
};