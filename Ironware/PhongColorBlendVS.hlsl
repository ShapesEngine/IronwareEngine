cbuffer CBuffer
{
    matrix modelView;
    matrix modelViewProjection;
};

struct VSOut
{
    float3 viewPos : Position;
    float3 n : Normal;
    float3 color : Color;    
    float4 pos : SV_Position;
};

VSOut main( float3 pos : Position, float3 n : Normal, float3 color : Color )
{
    float4 pos4 = float4( pos, 1.f );
    
    VSOut vso;
    vso.viewPos = mul( pos4, modelView );
    vso.n = n;
    vso.color = color;    
    vso.pos = mul( pos4, modelViewProjection );
    
    return vso;
}