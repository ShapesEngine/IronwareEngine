struct VSOut
{
    float3 color : COLOR;
    float4 pos : SV_POSITION;    
};

VSOut main( float2 pos : POSITION, float3 color : COLOR )
{
    VSOut outvs;
    outvs.pos = float4( pos.x, pos.y, 0.f, 1.f );
    outvs.color = color;
    
	return outvs;
}