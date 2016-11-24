#include "template.h"
#include "Camera.h"

Camera::Camera()
{
	center = position + d*view;
	p0 = center + vec3(-1, -1, 0);
	p1 = center + vec3(1, -1, 0);
	p2 = center + vec3(-1, 1, 0);
}