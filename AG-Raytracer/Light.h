#pragma once

class Light : public Entity
{
public:
	vec3 position;
	vec3 color;

	Triangle* tri;

	bool CheckIntersection(Ray* ray);

	Light(vec3 position, vec3 v1, vec3 v2, vec3 color) : Entity(position, true)
	{
		this->position = position;
		this->color = color;
		this->tri = new Triangle(position, v1, v2);
	}
};
