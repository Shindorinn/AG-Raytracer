#pragma once

class Primitive
{
public:
	//check glm documentation for rotation of camera.
	mat4 transformMatrix; // your transformation matrix.
	vec3 scale;
	//mat3 rotation;
	vec3 position;
	//vec3 skew;
	//vec4 perspective;

	vec3 centroid;

	Tmpl8::Material material = Tmpl8::Material(vec3(1, 0, 0), Tmpl8::Material::MaterialKind::DIFFUSE);
	Tmpl8::AABB* boundingBox;

	virtual bool CheckIntersection(Tmpl8::Ray* ray) = 0;
	virtual vec3 GetNormal(vec3 point) = 0;
	virtual Tmpl8::AABB* CalcAABB() = 0;

	Primitive(vec3 position);
	Primitive(mat4 transformMatrix);
	void Init(vec3 position);
	void Init(mat4 transformMatrix);

	vec3 GetPosition();
	vec3 GetDirectionVector();

	void UpdatePosition();

};
