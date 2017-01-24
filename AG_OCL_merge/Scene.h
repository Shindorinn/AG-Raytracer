#pragma once

#define TRI_SCENE 1
#define OBJ_LOAD 1
#define BUNNY_LOAD 1
#define SUZANNE_LOAD 0
#define f16_LOAD 0

class Scene
{
public:
	
#if TRI_SCENE
	Tmpl8::Primitive* primitives[9];
	Tmpl8::Light* lights[1];

#elif BUNNY_LOAD
	Tmpl8::Primitive* primitives[69632];
	Tmpl8::Light* lights[2];

#elif SUZANNE_LOAD
	Tmpl8::Primitive* primitives[15490];
	Tmpl8::Light* lights[2];

#elif f16_LOAD
	Tmpl8::Primitive* primitives[4058];
	Tmpl8::Light* lights[2];

#endif

	Tmpl8::Camera* camera;
	Tmpl8::AABB* sceneBounds;
	Tmpl8::BVH* bvh;
	Scene();

	Tmpl8::AABB* CalculateSceneBounds();
};