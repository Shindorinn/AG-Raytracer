#include "template.h"
#include "Light.h"

Light::Light(vec3 position, float intensity)
{
	this->position = position;
	this->intensity = intensity;
}