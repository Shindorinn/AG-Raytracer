#pragma once

class Primitive : public Entity
{
public:
	//check glm documentation for rotation of camera.
	mat4 transformMatrix; // your transformation matrix.
	vec3 scale;
	quat rotation;
	vec3 position;
	vec3 skew;
	vec4 perspective;

	vec3 centroid;

	Material material = Material(vec3(1, 0, 0), Material::MaterialKind::DIFFUSE);
	AABB* boundingBox;

	virtual bool CheckIntersection(Ray* ray) = 0;
	virtual vec3 GetNormal(vec3 point) = 0;
	virtual AABB* CalcAABB() = 0;

	Primitive(vec3 position);
	Primitive(mat4 transformMatrix);
	void Init(vec3 position);
	void Init(mat4 transformMatrix);

	vec3 GetPosition();
	vec3 GetDirectionVector();

	void UpdatePosition();

};
