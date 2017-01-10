#define PI				3.14159265358979323846f
#define INVPI			(1.0f / PI)
#define EPSILON			0.0001f

#define RAND()			RandomFloat(seed)

float3 Reflect( float3 A, float3 B ) 
{
	return A - 2.0f * B * dot( A, B ); 
}

float3 Refract( bool inside, float3 D, float3 N, uint* seed )
{
	float n1 = inside ? 1.2f : 1, n2 = inside ? 1 : 1.2f;
	float eta = n1 / n2, cosi = dot( -D, N ); 
	float cost2 = 1.0f - eta * eta * (1 - cosi * cosi);
	float3 R = Reflect( D, N );
	if (cost2 > 0)
	{
		float r1 = RAND();
		float a = n1 - n2, b = n1 + n2, R0 = (a * a) / (b * b), c = 1 - cosi;
		float Fr = R0 + (1 - R0) * (c * c * c * c * c);
		if (r1 > Fr) R = eta * D + ((eta * cosi - sqrt( fabs( cost2 ))) * N);
	}
	return R;
}

float3 DiffuseReflection( uint* seed )
{
	// based on SmallVCM / GIC
	float r1 = RAND(), r2 = RAND();
	float term1 = 2 * PI * r1;
	float term2 = 2 * sqrt( r2 * (1 - r2) );
	float3 R = (float3)( cos( term1 ) * term2, sin( term1 ) * term2, 1 - 2 * r2 );
	if (R.z < 0) R.z = -R.z;
	return R;
}

float3 Local2World( float3 V, float3 N )
{
	// based on SmallVCM
	float3 tmp = (fabs( N.x ) > 0.99f) ? (float3)( 0, 1, 0 ) : (float3)( 1, 0, 0 );
	float3 B = normalize( cross( N, tmp ) );
	float3 T = cross( B, N );
	return V.x * T + V.y * B + V.z * N;
}

float3 World2Local( float3 V, float3 N )
{
	float3 tmp = (fabs( N.x ) > 0.99f) ? (float3)( 0, 1, 0 ) : (float3)( 1, 0, 0 );
	float3 B = normalize( cross( N, tmp ) );
	float3 T = cross( B, N );
	return (float3)( dot( V, T ), dot( V, B ), dot( V, N ) );
}

float3 DiffuseReflectionCosWeighted( uint* seed )
{
	// based on SmallVCM
	float r0 = RAND(), r1 = RAND();
	float term1 = 2 * PI * r0;
	float term2 = sqrt( 1 - r1 );
	return (float3)( cos( term1 ) * term2, sin( term1 ) * term2, sqrt( r1 ) );
}