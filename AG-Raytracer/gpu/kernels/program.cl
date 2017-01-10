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