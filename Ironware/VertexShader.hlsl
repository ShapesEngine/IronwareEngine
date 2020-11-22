struct VSOut
{
    float3 color : COLOR;
    float4 pos : SV_POSITION;    
};

cbuffer CBuf
{
    row_major matrix transform;
};

VSOut main( float2 pos : POSITION, float3 color : COLOR )
{
    VSOut outvs;
    outvs.pos = mul(float4( pos.x, pos.y, 0.f, 1.f ), transform );
    outvs.color = color;
    
	return outvs;
}