#include "template.h"
#include "Primitive.h"

Primitive::Primitive(vec3 position)
{
	this->Init(position); 
};


Primitive::Primitive(mat4 transformMatrix)
{
	this->Init(transformMatrix);
};

void Primitive::Init(vec3 position)
{
	this->Init(
		mat4(
		1.0f, 0.0f, 0.0f, position.x,  // x0,y0,z0,w0
		0.0f, 1.0f, 0.0f, position.y,  // x1,y1,z1,w1
		0.0f, 0.0f, 1.0f, position.z,  // x2,y2,z2,w2 
		0.0f, 0.0f, 0.0f, 1.0f
		)
	); // x3,y3,z3,w3

};

void Primitive::Init(mat4 transformMatrix)
{
	this->transformMatrix = transformMatrix;
	this->scale = vec3();
	this->rotation = quat();
	this->position = vec3();
	this->skew = vec3();
	this->viewDirection = vec4();
};

vec3 Primitive::GetPosition()
{
	return (vec4(1, 0, 0, 0) * this->transformMatrix).xyz;
};

vec3 Primitive::GetDirectionVector()
{
	return this->GetPosition() + (vec4(0, 0, 1, 0) * this->transformMatrix).xyz;
};
