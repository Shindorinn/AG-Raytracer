#pragma once

struct AABB
{
	vec3 min;
	vec3 max;

	AABB(){};

	float GetVolume();

	AABB(vec3 min, vec3 max)
	{
		this->min = min;
		this->max = max;
	}
};