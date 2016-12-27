#include "template.h"

float AABB::GetVolume()
{
	float xwidth = this->max.x - this->min.x;
	float ywidth = this->max.y - this->min.y;
	float zwidth = this->max.z - this->min.z;

	return xwidth*ywidth*zwidth;
}