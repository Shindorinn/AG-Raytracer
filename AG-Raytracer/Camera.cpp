#include "template.h"
#include "Camera.h"
#include "Ray.h"
#include <random>

Camera::Camera()
{
	this->Init();
	this->d = 1;
	this->ratio = (float)SCRHEIGHT / (float)SCRWIDTH;
	this->width = 1.0f;
	this->height = ratio;

	//position = vec3(transformMatrix[0][3], transformMatrix[1][3], transformMatrix[2][3]);
	viewDirection = vec3(0.0f, 0.0f, 1.0f);

	printf("\n viewDirection : %f, %f, %f \n",
		viewDirection[0],
		viewDirection[1],
		viewDirection[2]
		);

	screenCenter = vec3(position.x, position.y, position.z) + d*viewDirection;
	p0 = screenCenter + vec3(-1.0f, -1.0f*this->ratio, 0.0f);
	p1 = screenCenter + vec3(1.0f, -1.0f *this->ratio, 0.0f);
	p2 = screenCenter + vec3(-1.0f, 1.0f *this->ratio, 0.0f);

	printf("\n screenCenter : %f, %f, %f \n",
		screenCenter[0],
		screenCenter[1],
		screenCenter[2]
		);
	printf("\n p0: %f, %f, %f \n",
		p0[0],
		p0[1],
		p0[2]
		);
	printf("\n p1: %f, %f, %f \n",
		p1[0],
		p1[1],
		p1[2]
		);
	printf("\n p2: %f, %f, %f \n",
		p2[0],
		p2[1],
		p2[2]
		);
}

void Camera::GenerateRays()
{
	float u, v = 0.0f;

	for (int y = 0; y < SCRHEIGHT; y++) {
		for (int x = 0; x < SCRWIDTH; x++)
		{
			u = (float) x / SCRWIDTH;
			v = (float) y / SCRHEIGHT;

			vec3 pointOnScreen = p0 + u*(p1 - p0) + v*(p2 - p0);

			vec3 position3d = position.xyz;
			vec3 pointOnScreen3d = pointOnScreen.xyz;

			vec3 direction = normalize(pointOnScreen3d - position3d);

			Ray* ray = new Ray(position3d, direction);

			if (y == SCRHEIGHT / 2 && x == SCRWIDTH / 2)
				printf("\n middle of the screen : %f, %f, %f \n", ray->direction[0], ray->direction[1], ray->direction[2]);

			primaryRays[y * SCRWIDTH + x] = ray;
		}
	}
}

void Camera::UpdateRays()
{
	float u, v = 0.0f;

	for (int y = 0; y < SCRHEIGHT; y++) {
		for (int x = 0; x < SCRWIDTH; x++)
		{
			u = (float)x / SCRWIDTH;
			v = (float)y / SCRHEIGHT;

			vec3 pointOnScreen = p0 + u*(p1 - p0) + v*(p2 - p0);

			vec3 position3d = position.xyz;
			vec3 pointOnScreen3d = pointOnScreen.xyz;

			vec3 direction = normalize(pointOnScreen3d - position3d);

			Ray* ray = primaryRays[y * SCRWIDTH + x];
			ray->direction = direction;
			ray->origin = position3d;
			ray->t = INFINITY;
		}
	}

}

//TODO : Check if the order of the matrix multiplication is correct
void Camera::TransformCamera(mat4 transformMatrix)
{
	this->transformMatrix = transformMatrix;
	this->UpdatePosition();
}

void Camera::Init()
{
	this->transformMatrix = mat4(1.0f);
	this->scale = vec3();
	this->rotation = quat();
	this->position = vec3();
	this->skew = vec3();
	this->perspective = vec4();
}

void Camera::UpdatePosition()
{
	decompose(transformMatrix, scale, rotation, position, skew, perspective);
	rotation = conjugate(rotation);

	screenCenter = vec3(position.x, position.y, position.z) + d*viewDirection;
	p0 = (transformMatrix * vec4(p0,1)).xyz;
	p1 = (transformMatrix * vec4(p1, 1)).xyz;
	p2 = (transformMatrix * vec4(p2, 1)).xyz;
}