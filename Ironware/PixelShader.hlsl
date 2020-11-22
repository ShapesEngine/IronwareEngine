float4 main(float3 color : COLOR) : SV_TARGET
{
    return float4(color, 1.f);
}