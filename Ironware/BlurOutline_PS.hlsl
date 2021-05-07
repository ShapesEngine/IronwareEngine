Texture2D tex;
SamplerState splr;

static const int r = 12;
static const float divisor = (float)( 2 * r + 1 ) * ( 2 * r + 1 );

float4 main( float2 uv : TEXCOORD ) : SV_TARGET
{
    uint width, height;
    tex.GetDimensions( width, height );
    const float dx = 1.f / (float)width;
    const float dy = 1.f / (float)height;
    float accAlpha = 0.0f;
    float3 maxColor = float3( 0.0f, 0.0f, 0.0f );
    for( int y = -r; y <= r; y++ )
    {
        for( int x = -r; x <= r; x++ )
        {
            const float2 tc = uv + float2( dx * x, dy * y );
            const float4 s = tex.Sample( splr, tc ).rgba;
            accAlpha += s.a;
            maxColor = max( s.rgb, maxColor );
        }
    }
    
    return float4( maxColor, accAlpha / divisor );
}