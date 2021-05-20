#include "CommonTransforms.hlsli"
#include "CommonShadowOps_VO.hlsli"

struct VSOut
{
    float3 viewPos : Position;
    float3 viewNormal : Normal;
    float2 tc : Texcoord;
    float4 shadowHomoPos : ShadowPosition;
    float4 pos : SV_Position;
};

VSOut main( float3 pos : Position, float3 n : Normal, float2 tc : Texcoord )
{
    VSOut vso;
    vso.viewPos = (float3)mul( float4( pos, 1.f ), modelView );
    vso.viewNormal = mul( n, (float3x3)modelView );
    vso.pos = mul( float4( pos, 1.f ), modelViewProjection );
    vso.tc = tc;
    vso.shadowHomoPos = to_shadow_homo_space( pos, model );
    return vso;
}