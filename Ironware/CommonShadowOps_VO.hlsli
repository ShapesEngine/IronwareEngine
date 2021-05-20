cbuffer ShadowTransformCBuf : register( b1 )
{
    matrix shadowViewProj;
};

float4 to_shadow_homo_space( const in float3 pos, uniform matrix modelTransform )
{
    const float4 world = mul( float4( pos, 1.f ), modelTransform );
    const float4 shadowHomo = mul( world, shadowViewProj );
    return shadowHomo * float4( 0.5f, -0.5f, 1.f, 1.f ) + float4( 0.5f, 0.5f, 0.f, 0.f ) * shadowHomo.w;
}