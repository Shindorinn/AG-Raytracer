#include "system.h"

float Tmpl8::AABB::GetVolume()
{
	float xwidth = this->max.x - this->min.x;
	float ywidth = this->max.y - this->min.y;
	float zwidth = this->max.z - this->min.z;

	return xwidth*ywidth*zwidth;
}

vec3 Tmpl8::AABB::GetCentroid()
{
	float x = ((this->max.x - this->min.x) / 2) + this->min.x;
	float y = ((this->max.y - this->min.y) / 2) + this->min.y;
	float z = ((this->max.z - this->min.z) / 2) + this->min.z;

	return vec3(x, y, z);
}