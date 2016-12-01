#pragma once

class Material
{
public:
	enum class MaterialKind { DIFFUSE, MIRROR, GLASS };
	Pixel color;
	MaterialKind materialKind;

	Material(Pixel color, MaterialKind materialKind)
	{
		this->color = color;
		this->materialKind = materialKind;
	}
protected:

private:

};