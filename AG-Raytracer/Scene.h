#pragma once

#define TRI_SCENE 0
#define TUNNEL_SCENE 0
#define MIRROR_SCENE 0
#define OBJ_LOAD 1

class Scene
{
public:
	
#if TRI_SCENE
	Primitive* primitives[9];
	Light* lights[1];
#elif TUNNEL_SCENE
	Primitive* primitives[8];
	Light* lights[2];
#elif MIRROR_SCENE
	Primitive* primitives[9];
	Light* lights[2];
#elif TINY_SCENE
	Primitive* primitives[1];
	Light* lights[2];
#elif OBJ_LOAD
	Primitive* primitives[69632];
	//cube 12, suzanne 15488, cruiser 2875, f16 4056, bunny 69630, bunnyLow: 4968
	Light* lights[2];
#endif

	Camera* camera;
	AABB* sceneBounds;
	BVH* bvh;
	Scene();

	AABB* CalculateSceneBounds();
};