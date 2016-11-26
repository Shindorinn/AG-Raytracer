#pragma once
#include "template.h"
#include "Ray.h"

class Camera
{
public:
	//mat4 check glm documentation for rotation of camera.
	mat4 transformMatrix;
	vec4 position;
	vec4 viewDirection;

	//TODO: TWEAK D FOR FOV
	float d = 1;

	vec4 screenCenter;
	vec4 p0, p1, p2;

	//TODO: Put this to renderer probably.
	Ray* primaryRays[SCRWIDTH*SCRHEIGHT];

	Camera();
	void GenerateRays();

	void TransformCamera(mat4 transformMatrix);
};
