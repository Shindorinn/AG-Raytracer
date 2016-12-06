#pragma once

class Material
{
public:
	enum class MaterialKind { DIFFUSE, MIRROR, GLASS };
	vec3 color;
	MaterialKind materialKind;

	float n1 = 1.0f;
	float n2 = 1.548f;
	float outToInN = n1 / n2;
	float inToOutN = n2 / n1;

	Material(vec3 color, MaterialKind materialKind)
	{
		this->color = color;
		this->materialKind = materialKind;
	}
protected:

private:

};