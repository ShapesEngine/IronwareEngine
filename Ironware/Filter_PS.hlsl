Texture2D tex;
SamplerState splr;

cbuffer Kernel
{
    uint nTaps;
    float coefficients[15];
}

cbuffer Control
{
    bool horizontal;
}

float4 main( float2 uv : TEXCOORD ) : SV_TARGET
{
    uint width, height;
    tex.GetDimensions( width, height );
    float dx = horizontal ? 1.f / (float)width : 0.f;
    float dy = horizontal ? 0.f : 1.f / (float)height;
    const int r = nTaps / 2;
    float4 acc = float4( 0.0f, 0.0f, 0.0f, 0.f );
    for( int i = -r; i <= r; i++ )
    {
        const float2 tc = uv + float2( dx * i, dy * i );
        const float4 s = tex.Sample( splr, tc ).rgba;
        const float coef = coefficients[i + r];
        acc += s * coef;
    }
    
    return acc;
}