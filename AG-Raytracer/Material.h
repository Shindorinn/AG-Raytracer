#pragma once

class Material
{
public:
	enum class MaterialKind { DIFFUSE, MIRROR, GLASS };
	vec3 color;
	MaterialKind materialKind;
	float n1 = 1.0f;	// Air
	float n2 = 1.458f;	// Fused Silica (fused quartz)
	float ndiv = n1 / n2;
	float ndiv2 = ndiv*ndiv;


	Material(vec3 color, MaterialKind materialKind)
	{
		this->color = color;
		this->materialKind = materialKind;
	}
protected:

private:

};