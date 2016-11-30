#pragma once
#include "Ray.h"

class Primitive
{
public:
	mat4 transformMatrix;

	virtual void CheckIntersection(Ray* ray) = 0;
	virtual vec3 GetNormal(vec3 point) = 0;
	
	Primitive(vec3 position);
	Primitive(mat4 transformMatrix);
	void Init(vec3 position);
	void Init(mat4 transformMatrix);
	
	vec3 GetPosition();
	vec3 GetDirectionVector();
};
