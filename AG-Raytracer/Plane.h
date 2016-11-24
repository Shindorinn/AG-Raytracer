#pragma once
#include "Primitive.h"

class Plane : Primitive
{
public:
	//p*N +d = 0

	Plane(vec3 position) : Primitive(position)
	{

	}
};
