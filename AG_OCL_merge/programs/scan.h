// from clpp: https://code.google.com/p/clpp

#define T int
#define OPERATOR_APPLY(A,B) A+B
#define OPERATOR_IDENTITY 0

//#define VOLATILE volatile
#define VOLATILE

inline T scan_exclusive( __local VOLATILE T* input, size_t idx, const uint lane )
{
	if (lane > 0 ) input[idx] = OPERATOR_APPLY( input[idx - 1] , input[idx] );
	if (lane > 1 ) input[idx] = OPERATOR_APPLY( input[idx - 2] , input[idx] );
	if (lane > 3 ) input[idx] = OPERATOR_APPLY( input[idx - 4] , input[idx] );
	if (lane > 7 ) input[idx] = OPERATOR_APPLY( input[idx - 8] , input[idx] );
	if (lane > 15) input[idx] = OPERATOR_APPLY( input[idx - 16], input[idx] );
	return (lane > 0) ? input[idx - 1] : OPERATOR_IDENTITY;
}

inline T scan_inclusive( __local VOLATILE T* input, size_t idx, const uint lane )
{	
	if (lane > 0 ) input[idx] = OPERATOR_APPLY( input[idx - 1] , input[idx] );
	if (lane > 1 ) input[idx] = OPERATOR_APPLY( input[idx - 2] , input[idx] );
	if (lane > 3 ) input[idx] = OPERATOR_APPLY( input[idx - 4] , input[idx] );
	if (lane > 7 ) input[idx] = OPERATOR_APPLY( input[idx - 8] , input[idx] );
	if (lane > 15) input[idx] = OPERATOR_APPLY( input[idx - 16], input[idx] );
	return input[idx];
}

inline T scan_workgroup_exclusive( __local T* temp, const uint idx, const uint lane, const uint warp )
{
	T val = scan_exclusive( temp, idx, lane );
	barrier( CLK_LOCAL_MEM_FENCE );
	if (lane > 30) temp[warp] = temp[idx];
	barrier( CLK_LOCAL_MEM_FENCE );
	if (warp < 1) scan_inclusive( temp, idx, lane );
	barrier( CLK_LOCAL_MEM_FENCE );
	if (warp > 0) val = OPERATOR_APPLY( temp[warp - 1], val );
	barrier( CLK_LOCAL_MEM_FENCE );
	temp[idx] = val;
	barrier( CLK_LOCAL_MEM_FENCE );
	return val;
}

__kernel void kernel__scan_block_anylength( __local T* temp, __global T* data, const uint B, uint N, const uint passes )
{	
	size_t idx = get_local_id( 0 );
	const uint bidx = get_group_id( 0 );
	const uint TC = get_local_size( 0 );
	const uint lane = idx & 31;
	const uint warp = idx >> 5;
	T reduceValue = OPERATOR_IDENTITY;
	// #pragma unroll 4
	for( uint i = 0; i < passes; ++i )
	{
		const uint offset = i * TC + (bidx * B);
		const uint offsetIdx = offset + idx;
		if (offsetIdx > (N - 1)) return;
		T input = temp[idx] = data[offsetIdx];
		barrier( CLK_LOCAL_MEM_FENCE );
		T val = scan_workgroup_exclusive( temp, idx, lane, warp );
		val = OPERATOR_APPLY( val, reduceValue );
		data[offsetIdx] = val;
		if (idx == (TC - 1)) temp[idx] = OPERATOR_APPLY( input, val );
		barrier( CLK_LOCAL_MEM_FENCE );
		reduceValue = temp[TC - 1];
		barrier( CLK_LOCAL_MEM_FENCE );
	}
}