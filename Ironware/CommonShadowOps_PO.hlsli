Texture2D smap : register( t3 );
SamplerState ssam : register( s1 );

bool shadow_unoccluded( const in float4 shadowHomoPos )
{
    const float3 spos = shadowHomoPos.xyz / shadowHomoPos.w;
    return spos.z > 1.f ? true : smap.Sample( ssam, spos.xy ).r > spos.z - 0.005f;
}