#pragma once
#include "Ray.h"

class Primitive
{
public: 
	vec3 position;
	
	virtual void CheckIntersection(Ray* ray) = 0;	
	
	Primitive(vec3 position);
};
