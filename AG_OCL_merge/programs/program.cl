#include "../shared.h"

typedef struct 
{
	float3 origin;
	float3 direction;
	float t; 
}
Ray;

typedef struct
{
	float3 pos;		// 16 bytes
	float3 target;	// 16 bytes
	float3 p1;		// 16 bytes
	float3 p2;		// 16 bytes
	float3 p3;		// 16 bytes
}
RenderData;

float intersectRayTriangle	(	
								Ray ray, 
								float3 v0, 
								float3 v1, 
								float3 v2
							)
{  

#define Epsilon 0.000001
	// Vectors from v0 to v1/v2 (edges)
	float3 e1, e2;
	float3 p, q, t;

	float det, invDet, u, v;

	//Find vectors for two edges sharing vertex/point v0
	e1 = v1 - v0;
	e2 = v2 - v0;

	// calculating determinant 
	p = cross(ray.direction, e2);

	//Calculate determinat
	det = dot(e1, p);

	//if determinant is near zero, ray lies in plane of triangle otherwise not
	if (det > -Epsilon && det < Epsilon) { return -1.0f; }
	invDet = 1.0f / det;

	//calculate distance from v0 to ray origin
	t = ray.origin - v0;

	//Calculate u parameter
	u = dot(t, p) * invDet;

	//Check for ray hit
	if (u < 0 || u > 1) { return -1.0f; }

	//Prepare to test v parameter
	q = cross(t, e1);

	//Calculate v parameter
	v = dot(ray.direction, q) * invDet;

	//Check for ray hit
	if (v < 0 || u + v > 1) { return -1.0f; }

	float tValue = dot(e2, q) * invDet;
	if (tValue > Epsilon)
	{
		//ray does intersect
		ray.t = tValue;
		return 1.0f;
	}

	// No hit at all
	return -1.0f;
}


Ray GeneratePrimaryRay( int x, int y, int width, int height, float3 pos, float3 target, float3 p1, float3 p2, float3 p3) //
{
	Ray r;
	float3 E = normalize( target - pos );
	float fx = (float)x / width;
	float fy = (float)y / height;
	float3 P = p1 + (p2 - p1) * fx + (p3 - p1) * fy;
	r.direction = normalize( P - E );
	r.origin = pos;
	r.t = 1e34f;
	return r;
}

float3 Trace(
				int x,
				int y,
				int width,
				int height,
				float3 pos,
				float3 target,
				float3 p1,
				float3 p2,
				float3 p3,
				int number_of_triangles,
				int number_of_lights,
				__global float4* triangleData,
				__global float4* lightData
				)
{

	Ray r = GeneratePrimaryRay(x, y, width, height, pos, target, p1, p2, p3);
	float smallestT = INFINITY;
	int indexHit = -1;
	for(int x = 0; x < number_of_triangles * 5; x+=5)
	{
		intersectRayTriangle(r, (float3)(triangleData[x]).xyz, (float3)(triangleData[x+1]).xyz, (float3)(triangleData[x+2]).xyz);
		if (r.t < smallestT)
		{
			smallestT = r.t;
			indexHit = x;
		}
	}

	float3 colorResult;
	if(indexHit > -1){
		colorResult = (float3)( 1.0f, 0.0f, 0.0f);
	} else {
		colorResult = (float3)( 0.0f, 0.0f, 1.0f);
	}
	
	r.t = INFINITY;

	return colorResult;
}


__kernel void WhittedStyleRender(	write_only image2d_t outimg,
									RenderData renderData,
									const int width,
									const int height,
									const int number_of_triangles,
									const int number_of_lights,
									__global float4* triangleData,
									__global float4* lightData
									)
{

	uint x = get_global_id( 0 );
	uint y = get_global_id( 1 );
	const uint pixelIdx = x + y * width;
	if (pixelIdx >= (width * height)) return;
	float3 color = Trace( x, y, width, height, renderData.pos, renderData.target, renderData.p1, renderData.p2, renderData.p3, number_of_triangles, number_of_lights, triangleData, lightData);
	int r = (int)(clamp( color.x, 0.f, 1.f ) * 255.0f);
	int g = (int)(clamp( color.y, 0.f, 1.f ) * 255.0f);
	int b = (int)(clamp( color.z, 0.f, 1.f ) * 255.0f);
	write_imagef( outimg, (int2)(x, y), (float4)(1, 0, 0, 1));
}



/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/

