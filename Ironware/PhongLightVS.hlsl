cbuffer CBuffer
{
    matrix model;
    matrix modelViewProjection;
};

struct VSOut
{
    float3 worldPos : Position;
    float3 normal : Normal;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 n : Normal)
{
    VSOut vso;
    vso.worldPos = (float3) mul(float4(pos, 1.f), model);
    vso.normal = mul(n, (float3x3) model);
    vso.pos = mul(float4(pos, 1.f), modelViewProjection);
    return vso;
}