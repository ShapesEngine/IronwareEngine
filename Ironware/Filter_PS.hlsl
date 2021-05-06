Texture2D tex;
SamplerState splr;

float4 main( float2 uv : TEXCOORD ) : SV_TARGET
{
    return 1.f - tex.Sample( splr, uv );
}