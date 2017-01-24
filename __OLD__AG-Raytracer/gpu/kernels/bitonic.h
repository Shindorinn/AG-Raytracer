// https://code.google.com/p/rtrt-on-gpu/source/browse/trunk/Source/OpenCL+Tutorial/Bitonic+Sort/Sort.cl?r=326

__kernel void Sort( __global uint* data, const uint stage, const uint passOfStage, const uint width, const uint direction )
{
	uint sortDir = direction;
	const uint idx = get_global_id( 0 );
	const uint pairDist = 1 << (stage - passOfStage);
	const uint leftId = (idx % pairDist) + (idx / pairDist) * 2 * pairDist;
	const uint rightId = leftId + pairDist;
	const uint A = data[leftId];
	const uint B = data[rightId];
	sortDir = ((idx >> stage) & 1) == 1 ? (1 - sortDir) : sortDir;
	const uint greater = A > B ? A : B;
	const uint lesser = A > B ? B : A;
	data[leftId] = sortDir ? lesser : greater;
	data[rightId] = sortDir ? greater : lesser;
}