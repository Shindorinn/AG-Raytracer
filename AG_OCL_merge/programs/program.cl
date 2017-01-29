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

float3 Trace(int x, int y, int width, int height, float3 pos, float3 target, float3 p1, float3 p2, float3 p3, int number_of_triangles, int number_of_lights){
	Ray r = GeneratePrimaryRay(x, y, width, height, pos, target, p1, p2, p3);

	return (float3)( 0.0f, 0.0f, 0.0f);
}


__kernel void WhittedStyleRender(	write_only image2d_t outimg,
									RenderData renderData,
									const int width,
									const int height,
									const int number_of_triangles,
									const int number_of_lights
									
									)
{

	uint x = get_global_id( 0 );
	uint y = get_global_id( 1 );
	const uint pixelIdx = x + y * width;
	if (pixelIdx >= (width * height)) return;
	float3 color = Trace( x, y, width, height, renderData.pos, renderData.target, renderData.p1, renderData.p2, renderData.p3, number_of_triangles, number_of_lights);
	int r = (int)(clamp( color.x, 0.f, 1.f ) * 255.0f);
	int g = (int)(clamp( color.y, 0.f, 1.f ) * 255.0f);
	int b = (int)(clamp( color.z, 0.f, 1.f ) * 255.0f);
	write_imagef( outimg, (int2)(x, y), (float4)(1, 0, 0, 1));
}




/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/

