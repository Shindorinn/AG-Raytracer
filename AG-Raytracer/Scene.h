#pragma once

#define TRI_SCENE 0
#define OBJ_LOAD 0
#define TUNNEL_SCENE 1
#define BUNNY_LOAD 0
#define SUZANNE_LOAD 0
#define f16_LOAD 0

class Scene
{
public:

#if TRI_SCENE
	Primitive* primitives[9];
	Light* lights[1];

#elif TUNNEL_SCENE
	Primitive* primitives[7];
	Light* lights[2];
	Entity* entities[9];

#elif BUNNY_LOAD
	Primitive* primitives[69632];
	Light* lights[2];

#elif SUZANNE_LOAD
	Primitive* primitives[15490];
	Light* lights[2];

#elif f16_LOAD
	Primitive* primitives[4058];
	Light* lights[2];

#endif

	Camera* camera;
	AABB* sceneBounds;
	BVH* bvh;
	Scene();

	AABB* CalculateSceneBounds();
};