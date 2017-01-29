#include "../shared.h"
#include "randomnumbers.h"
#include "program.h"

/*
// intersection code --------------------------------------------------
int RayBoxIntersection(		const float3 &BBMin,
							const float3 &BBMax,
							const float3 &RayOrg,
							const float3 &RayDirInv, // 1 / .x, 1/ .y, 1/ .z
							float &tmin,
							float &tmax)
{
	float l1   = (BBMin.x - RayOrg.x) * RayDirInv.x;
	float l2   = (BBMax.x - RayOrg.x) * RayDirInv.x;
	tmin = fminf(l1,l2);
	tmax = fmaxf(l1,l2);

	l1   = (BBMin.y - RayOrg.y) * RayDirInv.y;
	l2   = (BBMax.y - RayOrg.y) * RayDirInv.y;
	tmin = fmaxf(fminf(l1,l2), tmin);
	tmax = fminf(fmaxf(l1,l2), tmax);

	l1   = (BBMin.z - RayOrg.z) * RayDirInv.z;
	l2   = (BBMax.z - RayOrg.z) * RayDirInv.z;
	tmin = fmaxf(fminf(l1,l2), tmin);
	tmax = fminf(fmaxf(l1,l2), tmax);

	return ((tmax >= tmin) && (tmax >= 0.0f));
}

// the classic ray triangle intersection: http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
float RayTriangleIntersection(	const Ray &r, 
								const float3 &v0, 
								const float3 &edge1, 
								const float3 &edge2)
{  

	float3 tvec = r.origin - v0;  
	float3 pvec = cross(r.direction, edge2);  
	float  det  = dot(edge1, pvec);  

	det = __fdividef(1.0f, det);  

	float u = dot(tvec, pvec) * det;  

	if (u < 0.0f || u > 1.0f)  
		return -1.0f;  

	float3 qvec = cross(tvec, edge1);  

	float v = dot(r.direction, qvec) * det;  

	if (v < 0.0f || (u + v) > 1.0f)  
		return -1.0f;  

	return dot(edge2, qvec) * det;  
}  

__device__ int RaySphereIntersection(const Ray &ray, const float3 sphere_location, const float sphere_radius, float &t)
{
	float b, c, d;

	float3 sr = ray.origin - sphere_location;
	b =  dot(sr,ray.dir);
	c = dot(sr,sr) - (sphere_radius*sphere_radius);
	d = b*b - c;
	if (d > 0) 
	{
		float e = sqrt(d);
		float t0 = -b-e;
		if(t0 < 0)
			t = -b+e;
		else
			t = min(-b-e,-b+e);
		return 1;
	}
	return 0;
}

*/

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

	return make_float3( 0, 0, 0);
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

/*

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

	return float3( 0, 0, 0);
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

*/

/*

// intersection code --------------------------------------------------
__device__ int RayBoxIntersection(const float3 &BBMin, const float3 &BBMax, const float3 &RayOrg, const float3 &RayDirInv, float &tmin, float &tmax)
{
	float l1   = (BBMin.x - RayOrg.x) * RayDirInv.x;
	float l2   = (BBMax.x - RayOrg.x) * RayDirInv.x;
	tmin = fminf(l1,l2);
	tmax = fmaxf(l1,l2);

	l1   = (BBMin.y - RayOrg.y) * RayDirInv.y;
	l2   = (BBMax.y - RayOrg.y) * RayDirInv.y;
	tmin = fmaxf(fminf(l1,l2), tmin);
	tmax = fminf(fmaxf(l1,l2), tmax);

	l1   = (BBMin.z - RayOrg.z) * RayDirInv.z;
	l2   = (BBMax.z - RayOrg.z) * RayDirInv.z;
	tmin = fmaxf(fminf(l1,l2), tmin);
	tmax = fminf(fmaxf(l1,l2), tmax);

	return ((tmax >= tmin) && (tmax >= 0.0f));
}

// the classic ray triangle intersection: http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
__device__ float RayTriangleIntersection(const Ray &r, 
										 const float3 &v0, 
										 const float3 &edge1, 
										 const float3 &edge2)
{  

	float3 tvec = r.origin - v0;  
	float3 pvec = cross(r.direction, edge2);  
	float  det  = dot(edge1, pvec);  

	det = __fdividef(1.0f, det);  

	float u = dot(tvec, pvec) * det;  

	if (u < 0.0f || u > 1.0f)  
		return -1.0f;  

	float3 qvec = cross(tvec, edge1);  

	float v = dot(r.direction, qvec) * det;  

	if (v < 0.0f || (u + v) > 1.0f)  
		return -1.0f;  

	return dot(edge2, qvec) * det;  
}  

__device__ int RaySphereIntersection(const Ray &ray, const float3 sphere_location, const float sphere_radius, float &t)
{
	float b, c, d;

	float3 sr = ray.origin - sphere_location;
	b =  dot(sr,ray.dir);
	c = dot(sr,sr) - (sphere_radius*sphere_radius);
	d = b*b - c;
	if (d > 0) 
	{
		float e = sqrt(d);
		float t0 = -b-e;
		if(t0 < 0)
			t = -b+e;
		else
			t = min(-b-e,-b+e);
		return 1;
	}
	return 0;
}

*/




/*
struct Ray {
	__device__ Ray(){};
	__device__ Ray(const float3 &o,const float3 &d)
	{
		origin = o;
		direction = d;
		direction = normalize(direction);
	}
	float3 origin;
	float3 direction;
	float dummy;
	float dummy1,dummy2;
} ;

struct Hit{
	__device__ Hit() {t = UINT_MAX;hitIndex = -1; color = make_float3(0,0,0);}
	__device__ void resetT(){t = UINT_MAX; hitIndex = -1;}
	float t;
	float3 color;
	float3 normal;
	int hitIndex;
};

*/


/*
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
*/

/*
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
*/

/*
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
*/



/*

Ray GeneratePrimaryRay( int x, int y, int width, int height, float3 pos, float3 target, float3 p1, float3 p2, float3 p3) //
{
	Ray r;
	float3 E = normalize( target - pos );
	//float3 up = (float3)( 0, 1, 0 );
	//float3 right = normalize( cross( up, E ) );
	//up = cross( E, right );
	//float3 C = pos + E;
	//float3 p1 = C - right + up; // top left
	//float3 p2 = C + right + up; // top right
	//float3 p3 = C - right - up; // bottom left
	float fx = (float)x / SCRWIDTH;
	float fy = (float)y / SCRHEIGHT;
	float3 P = p1 + (p2 - p1) * fx + (p3 - p1) * fy;
	r.direction = normalize( P - E );
	r.origin = pos;
	r.t = 1e34f;
	return r;
}

*/


/*
__global__ void WhittedStyleRender( unsigned int *out_data,
									RenderData renderData,
									const int width,
									const int height,
									const int number_of_triangles,
									const float3 a, const float3 b, const float3 c, 
									const float3 camera_pos,
									const float3 light_pos,
									const float3 light_color,
									const float3 scene_aabb_min, 
									const float3 scene_aabb_max)



*/



/* Aila & Laine Kernel
// global variables
const int B = 3*32; // example batch size
const int globalPoolRayCount;
int globalPoolNextRay = 0;
__global__ void kernel()
// variables shared by entire warp, place to shared memory
__shared__ volatile int nextRayArray[BLOCKDIM_Y];
__shared__ volatile int rayCountArray[BLOCKDIM_Y] = {0};
volatile int& localPoolNextRay = nextRayArray[threadIdx.y];
volatile int& localPoolRayCount = rayCountArray[threadIdx.y];
while (true) {
// get rays from global to local pool
if (localPoolRayCount==0 && threadIdx.x==0) {
localPoolNextRay = atomicAdd(globalPoolNextRay, B);
localPoolRayCount = B; }
// get rays from local pool
int myRayIndex = localPoolNextRay + threadIdx.x;
if (myRayIndex >= globalPoolRayCount)
return;
if (threadIdx.x==0) {
localPoolNextRay += 32;
localPoolRayCount -= 32; }
// init and execute, these must not exit the kernel
fetchAndInitRay(myRayIndex);
trace();
}

*/