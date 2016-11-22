#pragma once
#include "Ray.h"

class Primitive
{
public: 
	virtual void CheckIntersection(Ray ray) = 0;	
};
