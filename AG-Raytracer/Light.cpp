#include "template.h"

bool Light::CheckIntersection(Ray* ray)
{
	return this->tri->CheckIntersection(ray);
}