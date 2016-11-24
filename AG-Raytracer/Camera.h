#pragma once
#include "Ray.h"

class Camera
{
public:
	//mat4  check glm documentation for rotation of camera.
	vec3 position = vec3(0, 0, 0);
	vec3 view = vec3(0, 0, 1);

	//TODO: TWEAK D FOR FOV
	float d = 1;

	vec3 center;
	vec3 p0, p1, p2;

	//TODO: Put this to renderer probably.
	Ray* primaryRays[SCRWIDTH*SCRHEIGHT];

	Camera();
	void GenerateRays();
};
