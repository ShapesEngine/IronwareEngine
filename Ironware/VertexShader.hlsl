struct VSOut
{
    float3 color : COLOR;
    float4 pos : SV_POSITION;    
};

cbuffer CBuf
{
    matrix transform;
};

VSOut main( float3 pos : POSITION, float3 color : COLOR )
{
    VSOut outvs;
    outvs.pos = mul(float4( pos, 1.f ), transform );
    outvs.color = color;
    
	return outvs;
}