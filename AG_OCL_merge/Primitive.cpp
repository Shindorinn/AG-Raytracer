#include "system.h"

Tmpl8::Primitive::Primitive(vec3 position)
{
	this->Init(position); 
};


Tmpl8::Primitive::Primitive(mat4 transformMatrix)
{
	this->Init(transformMatrix);
};

void Tmpl8::Primitive::Init(vec3 position)
{
	this->Init(
		mat4::translate(position)
		//vec4(position, 1.0f) * mat4::identity()
	); // x3,y3,z3,w3

};

void Tmpl8::Primitive::Init(mat4 transformMatrix)
{
	this->transformMatrix = transformMatrix;
	this->scale = vec3();
	//this->rotation = quat();
	this->position = vec3();
	//this->skew = vec3();
	//this->perspective = vec4();
	this->UpdatePosition();
};

vec3 Tmpl8::Primitive::GetPosition()
{
	return this->position;
};

vec3 Tmpl8::Primitive::GetDirectionVector()
{
	return vec3(1, 0, 0);
};

void Tmpl8::Primitive::UpdatePosition()
{
	//decompose(transformMatrix, scale, rotation, position, skew, perspective);

	//rotation = conjugate(rotation);
}
