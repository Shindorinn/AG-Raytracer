#pragma once

class Material
{
public:
	enum class MaterialKind { DIFFUSE, MIRROR, GLASS };
	vec3 color;
	MaterialKind materialKind;

	Material(vec3 color, MaterialKind materialKind)
	{
		this->color = color;
		this->materialKind = materialKind;
	}
protected:

private:

};