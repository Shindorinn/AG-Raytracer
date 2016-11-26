#pragma once
#include "template.h"
#include "Primitive.h"
#include "Material.h"
#include "Texture.h"


class _3DObject
{

public:
	Primitive** primitives;
	Material material;
	Texture texture;

	mat4 transformMatrix;

	_3DObject();

protected:

private:

};

