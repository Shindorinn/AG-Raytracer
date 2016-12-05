#pragma once
#include "template.h"
#include "Ray.h"

class Camera
{
public:
	//check glm documentation for rotation of camera.
	mat4 transformMatrix; // your transformation matrix.
	vec3 position;
	vec3 wUp = vec3(0, 1, 0);
	vec3 rUp;
	vec3 rRight;

	vec3 viewDirection;
	vec3 screenCenter;
	vec3 p0, p1, p2;

	//TODO: TWEAK D FOR FOV
	float d;
	float width;
	float height;
	float ratio;

	//TODO: Put this to renderer probably.
	Ray* primaryRays[SCRWIDTH*SCRHEIGHT];

	Camera();
	void Init();
	void GenerateRays();
	void UpdateRays();
	void TransformCamera(mat4 transformMatrix);
	void UpdatePosition();
};
