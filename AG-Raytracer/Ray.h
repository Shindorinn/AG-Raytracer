#pragma once

class Ray
{
public:
	vec3 origin;
	vec3 direction;	
	float t;
	Ray(vec3 origin, vec3 direction);
};