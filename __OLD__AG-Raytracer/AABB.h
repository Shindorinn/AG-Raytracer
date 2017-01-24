#pragma once

struct AABB
{
	vec3 min;
	vec3 max;

	vec3 centroid;

	AABB() {};

	float GetVolume();
	vec3 GetCentroid();

	AABB(vec3 min, vec3 max)
	{
		this->min = min;
		this->max = max;
		this->centroid = GetCentroid();
	}
};