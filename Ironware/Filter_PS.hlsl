Texture2D tex;
SamplerState splr;

static const int r = 3;
static const float divisor = (2 * r + 1) * (2 * r + 1);

float4 main( float2 uv : TEXCOORD ) : SV_TARGET
{
    uint width, height;
    tex.GetDimensions( width, height );
    const float dx = 1.f / (float)width;
    const float dy = 1.f / (float)height;
    float4 acc = float4( 0.f, 0.f, 0.f, 0.f );
    for( int y = -r; y <= r; y++ )
    {
        for( int x = -r; x <= r; x++ )
        {
            const float2 tc = uv + float2( dx * x, dy * y );
            acc += tex.Sample( splr, tc );
        }
    }
    
    return acc / divisor;
}