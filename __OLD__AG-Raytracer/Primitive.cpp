#include "template.h"

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
		translate(mat4(1.0f), position)
	); // x3,y3,z3,w3

};

void Primitive::Init(mat4 transformMatrix)
{
	this->transformMatrix = transformMatrix;
	this->scale = vec3();
	this->rotation = quat();
	this->position = vec3();
	this->skew = vec3();
	this->perspective = vec4();
	this->UpdatePosition();
};

vec3 Primitive::GetPosition()
{
	return this->position;
};

vec3 Primitive::GetDirectionVector()
{
	return rotation * vec3(1, 0, 0);
};

void Primitive::UpdatePosition()
{
	decompose(transformMatrix, scale, rotation, position, skew, perspective);
	rotation = conjugate(rotation);
}
