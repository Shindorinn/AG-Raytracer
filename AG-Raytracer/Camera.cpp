#include "template.h"
#include "Camera.h"
#include "Ray.h"

Camera::Camera()
{
	center = position + d*view;
	p0 = center + vec3(-1, -1, 0);
	p1 = center + vec3(1, -1, 0);
	p2 = center + vec3(-1, 1, 0);
}


void Camera::GenerateRays()
{
	float u, v = 0.0f;

	float ratio = SCRWIDTH / SCRHEIGHT;
	float width = 1.0f;
	float height = ratio;
	
	for (int y = 0; y < SCRHEIGHT; y++){
		for (int x = 0; x < SCRWIDTH; x++)
		{
			u = (width / SCRWIDTH) * x;
			v = (height / SCRHEIGHT) * y;

			vec3 pointOnScreen = p0 + u*(p1 - p0) + v*(p2 - p0);
			vec3 direction = normalize(pointOnScreen - position);

			Ray* ray = new Ray(position, direction);

			//printf("%f, %f, %f \n", ray->direction[0], ray->direction[1], direction, ray->direction[2]);

			primaryRays[y * SCRWIDTH + x] = ray;
		}
	}

	/*for (int y = 0; y < SCRHEIGHT; y++)
		for (int x = 0; x < SCRWIDTH; x++)
		{
			u = float(x) / SCRWIDTH;
			v = float(y) / SCRHEIGHT;

			vec3 pointOnScreen = p0 + u*(p1 - p0) + v*(p2 - p0);

			vec3 direction = normalize(pointOnScreen - position);

			Ray* ray = new Ray(position, direction);

			printf("%f, %f, %f \n", ray->direction[0], ray->direction[1], direction, ray->direction[2]);

			primaryRays[y * SCRWIDTH + x] = ray;

		}*/
}