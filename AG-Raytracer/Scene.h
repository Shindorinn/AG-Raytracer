#pragma once

#define TRI_SCENE 0
#define OBJ_LOAD 1
#define TUNNEL_SCENE 0
#define BUNNY_LOAD 1
#define SUZANNE_LOAD 0
#define f16_LOAD 0

class Scene
{
public:

#if TRI_SCENE
	Primitive* primitives[9];
	Light* lights[2];
	Entity* entities[11];

#elif TUNNEL_SCENE
	Primitive* primitives[13];
	Light* lights[2];
	Entity* entities[15];

	//Primitive* primitives[8];
	//Entity* entities[10];

#elif BUNNY_LOAD
	Primitive* primitives[69632];
	Light* lights[2];
	Entity* entities[69634];

#elif SUZANNE_LOAD
	Primitive* primitives[15490];
	Light* lights[2];
	Entity* entities[15492];

#elif f16_LOAD
	Primitive* primitives[4058];
	Light* lights[2];
	Entity* entities[4060];

#endif

	Camera* camera;
	AABB* sceneBounds;
	BVH* bvh;
	Scene();

	int lightCount;
	int primCount;

	AABB* CalculateSceneBounds();
};