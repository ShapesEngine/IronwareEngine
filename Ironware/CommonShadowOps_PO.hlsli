Texture2D smap : register( t3 );
SamplerComparisonState ssam : register( s1 );

cbuffer ShadowControl : register( b2 )
{
    int pcfLevel;
    float depthBias;
}

float shadow_loop( const in float3 spos, uniform int range )
{
    float shadowLevel = 0.0f;
    [unroll]
    for( int x = -range; x <= range; x++ )
    {
        [unroll]
        for( int y = -range; y <= range; y++ )
        {
            shadowLevel += smap.SampleCmpLevelZero( ssam, spos.xy, spos.b - depthBias, int2( x, y ) );
        }
    }
    return shadowLevel / ( ( range * 2 + 1 ) * ( range * 2 + 1 ) );
}

bool shadow( const in float4 shadowHomoPos )
{
    float shadowLevel = 0.f;
    const float3 spos = shadowHomoPos.xyz / shadowHomoPos.w;
    
    if( spos.z > 1.f || spos.z < 0.f )
    {
        shadowLevel = 1.f;
    }
    else
    {
        [unroll]
        for( int level = 0; level <= 4; level++ )
        {
            if( level == pcfLevel )
            {
                shadowLevel = shadow_loop( spos, level );
            }
        }
    }
    return shadowLevel;
}