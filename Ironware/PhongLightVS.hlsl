cbuffer CBuffer
{
    matrix modelView;
    matrix modelViewProjection;
};

struct VSOut
{
    float3 viewPos : Position;
    float3 normal : Normal;
    float4 pos : SV_Position;
};

VSOut main( float3 pos : Position, float3 n : Normal )
{
    VSOut vso;
    vso.viewPos = (float3)mul( float4( pos, 1.f ), modelView );
    vso.normal = mul( n, (float3x3)modelView );
    vso.pos = mul( float4( pos, 1.f ), modelViewProjection );
    return vso;
}