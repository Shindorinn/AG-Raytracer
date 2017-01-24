#pragma once

class Renderer
{
public:
	Tmpl8::Surface* renderSurface;
	Tmpl8::Scene* scene;
	Tmpl8::Pixel buffer[SCRHEIGHT][SCRWIDTH];
	Renderer(Tmpl8::Scene* scene, Tmpl8::Surface* renderSurface);

	void Render();
	vec3 Trace(Tmpl8::Ray* ray, int x, int y);
	vec3 DirectIllumination(vec3 intersectionPoint, vec3 direction, vec3 normal, Tmpl8::Light* lightSource, Tmpl8::Material material);

	vec3 Reflect(vec3 direction, vec3 normal);
};
