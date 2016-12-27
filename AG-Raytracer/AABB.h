#pragma once

struct AABB
{
	//TODO: Maybe just use floats if vec3 uses more than 3float size.
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