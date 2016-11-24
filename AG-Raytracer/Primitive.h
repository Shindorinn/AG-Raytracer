#pragma once
#include "Ray.h"

class Primitive
{
public: 
	virtual void CheckIntersection(Ray ray) = 0;	
	vec3 position;

	Primitive(vec3 position);
};
