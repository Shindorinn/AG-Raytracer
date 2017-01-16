//#include "template.h"
//
//Camera::Camera()
//{
//	this->Init();
//}
//
//void Camera::GenerateRays()
//{
//	float u, v = 0.0f;
//	for (int y = 0; y < SCRHEIGHT; y++) {
//		for (int x = 0; x < SCRWIDTH; x++)
//		{
//			u = (float) x / SCRWIDTH;
//			v = (float) y / SCRHEIGHT;
//
//			vec3 pointOnScreen = p0 + u*(p1 - p0) + v*(p2 - p0);
//
//			vec3 position3d = position.xyz;
//			vec3 pointOnScreen3d = pointOnScreen.xyz;
//
//			vec3 direction = normalize(pointOnScreen3d - position3d);
//
//			Ray* ray = new Ray(position3d, direction);
//
//			primaryRays[y * SCRWIDTH + x] = ray;
//		}
//	}
//}
//
//void Camera::UpdateRays()
//{
//	float u, v = 0.0f;
//	#pragma omp parallel for
//	for (int y = 0; y < SCRHEIGHT; y++) {
//		#pragma omp parallel for
//		for (int x = 0; x < SCRWIDTH; x++)
//		{
//			u = (float)x / SCRWIDTH;
//			v = (float)y / SCRHEIGHT;
//
//			vec3 pointOnScreen = p0 + u*(p1 - p0) + v*(p2 - p0);
//
//			vec3 position3d = position.xyz;
//			vec3 pointOnScreen3d = pointOnScreen.xyz;
//
//			vec3 direction = normalize(pointOnScreen3d - position3d);
//
//			Ray* ray = primaryRays[y * SCRWIDTH + x];
//			ray->direction = direction;
//			ray->origin = position3d;
//			ray->t = INFINITY;
//		}
//	}
//}
//
//void Camera::TransformCamera(mat4 transformMatrix)
//{
//	this->transformMatrix = transformMatrix;
//	this->UpdatePosition();
//	this->UpdateRays();
//}
//
//void Camera::Init()
//{
//	this->wUp = vec3(0, 1, 0);
//	this->viewDirection = vec3(0.0f, 0.0f, 1.0f);
//	this->position = vec3(0, 0, -5);
//
//	this->d = 1;
//	this->ratio = (float)SCRHEIGHT / (float)SCRWIDTH;
//	this->width = 1.0f;
//	this->height = ratio;
//
//	this->screenCenter = vec3(position.x, position.y, position.z) + d*viewDirection;
//	this->p0 = screenCenter + vec3(-1.0f, -1.0f*this->ratio, 0.0f);
//	this->p1 = screenCenter + vec3(1.0f, -1.0f *this->ratio, 0.0f);
//	this->p2 = screenCenter + vec3(-1.0f, 1.0f *this->ratio, 0.0f);
//	
//	this->transformMatrix = inverse(lookAt(position, screenCenter, wUp));	
//	this->rRight = vec3(this->transformMatrix[0]);
//	this->rUp = vec3(this->transformMatrix[1]);
//	this->viewDirection = vec3(this->transformMatrix[2]);
//}
//
//void Camera::UpdatePosition()
//{
//	this->viewDirection = vec3(this->transformMatrix[2]);
//	this->position = vec3(this->transformMatrix[3]);
//	this->rUp = vec3(this->transformMatrix[1]);
//	this->rRight = vec3(this->transformMatrix[0]);
//
//	this->screenCenter = vec3(position.x, position.y, position.z) + d*viewDirection;
//	this->p0 = (transformMatrix * vec4(p0, 1)).xyz;
//	this->p1 = (transformMatrix * vec4(p1, 1)).xyz;
//	this->p2 = (transformMatrix * vec4(p2, 1)).xyz;
//#if DEBUG
//	printf("\n position : %f, %f, %f \n",
//		position[0],
//		position[1],
//		position[2]
//	);
//	printf("\n viewDirection : %f, %f, %f \n",
//		viewDirection[0],
//		viewDirection[1],
//		viewDirection[2]
//	);
//	printf("\n screenCenter : %f, %f, %f \n",
//		screenCenter[0],
//		screenCenter[1],
//		screenCenter[2]
//	);
//	printf("\n p0: %f, %f, %f \n",
//		p0[0],
//		p0[1],
//		p0[2]
//	);
//	printf("\n p1: %f, %f, %f \n",
//		p1[0],
//		p1[1],
//		p1[2]
//	);
//	printf("\n p2: %f, %f, %f \n",
//		p2[0],
//		p2[1],
//		p2[2]
//	);
//#endif
//}