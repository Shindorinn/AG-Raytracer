#pragma once

//Entity is either a Light, or a Primitive.
class Entity
{
public:

	vec3 position;
	bool isLight;

	virtual bool CheckIntersection(Ray* ray) = 0;

	Entity(vec3 position, bool isLight);
};
