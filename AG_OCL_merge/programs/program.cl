#include "../shared.h"
#include "randomnumbers.h"
#include "program.h"

__kernel void TestFunction( write_only image2d_t outimg )
{
	uint x = get_global_id( 0 );
	uint y = get_global_id( 1 );
	const uint pixelIdx = x + y * SCRWIDTH;
	if (pixelIdx >= (SCRWIDTH * SCRHEIGHT)) return;
	float r = (float)x / SCRWIDTH;
	float g = (float)y / SCRHEIGHT;
	write_imagef( outimg, (int2)(x, y), (float4)( r, g, 0, 1 ) );
}

__kernel void WhittedStyleRender( write_only image2d_t outimg, float3 origin, float3 direction, float t, float3* triangles )
{
	uint x = get_global_id( 0 );
	uint y = get_global_id( 1 );
	const uint pixelIdx = x + y * SCRWIDTH;
	if (pixelIdx >= (SCRWIDTH * SCRHEIGHT)) return;
	Hit* hit = Trace(new Ray(origin, direction, t), triangles);
	if(hit->t < INFINITY)
	write_imagef( outimg, (int2)(x, y), (float4)(1, 0, 0, 1));
}

struct{
	float3 origin;
	float3 direction;
	float t;
} Ray;

struct{
	float3 intersectionPoint;
	int firstVertexIndex;
	float t;
} Hit;

float3 Trace(Ray* ray, float3* triangles)
{
	float smallestT = INFINITY;
	Hit* hit = new Hit(float3(0), -1, INFINITY);
	for (int x = 0; x < sizeof(triangles) / sizeof(triangles[0]); x+=3)
	{
		if (this->scene->entities[x]->CheckIntersection(ray, triangles[x], triangles[x+1], triangles[x+2]) && smallestT > ray->t)
		{
			smallestT = ray->t;
			hit->firstVertexIndex = x;
		}
	}
	//Set t back, this is needed for the pathtracing code which checks if we need to return black (occlusion toward light)
	float3 intersectionPoint = ray->origin + smallestT*ray->direction;
	hit->intersectionPoint = intersectionPoint;
	return hit;
}