struct VSOut
{
    float2 uv : TEXCOORD;
    float4 pos : SV_POSITION;
};

VSOut main( float2 pos : POSITION )
{
    VSOut vso;
    vso.pos = float4( pos, 0.f, 1.f );
    vso.uv = float2( ( pos.x + 1.f ) / 2.f, -( pos.y - 1.f ) / 2.f );
    return vso;
}