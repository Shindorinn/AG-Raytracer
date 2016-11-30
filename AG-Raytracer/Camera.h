#pragma once
#include "template.h"
#include "Ray.h"

class Camera
{
public:
	//check glm documentation for rotation of camera.
	mat4 transformMatrix; // your transformation matrix.
	vec3 scale;
	quat rotation;
	vec3 position;
	vec3 skew;
	vec4 viewDirection;

	//TODO: TWEAK D FOR FOV
	float d = 1;

	vec4 screenCenter;
	vec4 p0, p1, p2;

	//TODO: Put this to renderer probably.
	Ray* primaryRays[SCRWIDTH*SCRHEIGHT];

	Camera();
	void Init();
	void GenerateRays();
	void TransformCamera(mat4 transformMatrix);
	void UpdatePosition();
};
