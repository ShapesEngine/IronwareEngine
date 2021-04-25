#include "CommonTransforms.hlsli"

struct VSOut
{
    float3 viewPos : Position;
    float3 viewN : Normal;
    float2 tex : TexCoord;
    float3 viewTan : Tangent;
    float3 viewBitan : Bitangent;
    float4 pos : SV_Position;
};

VSOut main( float3 pos : Position, float3 n : Normal, float2 tex : TexCoord, float3 tan : Tangent, float3 bitan : Bitangent )
{
    float4 pos4 = float4( pos, 1.f );
    
    VSOut vso;
    vso.viewPos = (float3)mul( pos4, modelView );
    vso.viewN = mul( n, (float3x3)modelView );
    vso.tex = tex;
    vso.viewTan = mul( tan, (float3x3)modelView );
    vso.viewBitan = mul( bitan, (float3x3)modelView );
    vso.pos = mul( pos4, modelViewProjection );
    
    return vso;
}