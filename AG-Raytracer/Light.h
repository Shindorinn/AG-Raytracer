#pragma once
#include "template.h"

class Light
{
public:
	vec3 position;
	vec3 color;
	float intensity;
	Light(vec3 position, float intensity);
};
