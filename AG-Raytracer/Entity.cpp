#include "template.h"
#include "Entity.h"

Entity::Entity(vec3 position, bool isLight)
{
	this->position = position;
	this->isLight = isLight;
};
