Texture2D tex;
Texture2D nmap : register( t2 );

SamplerState splr;

// for global dynamic light
cbuffer LightCBuf
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};

// for object color
cbuffer SpecularCBuf
{
    float specularIntensity;
    float specularPower;
    bool isNMapEnabled;
};

cbuffer CBuffer
{
    matrix modelView;
    matrix modelViewProjection;
};

float4 main( float3 viewPos : Position, float3 viewN : Normal, float2 tc : TexCoord, float3 viewTan : Tangent, float3 viewBitan : Bitangent ) : SV_Target
{
    if( isNMapEnabled )
    {
        const float3x3 tanToView = float3x3(
            normalize( viewTan ),
            normalize( viewBitan ),
            normalize( viewN )
        );
        
        const float3 sampledNMap = (float3)nmap.Sample( splr, tc );
        viewN = 2.0f * sampledNMap - 1.0f;
        viewN.y = -viewN.y;
        viewN = mul( viewN, tanToView );
    }
    // fragment to light vector data
    const float3 vToL = lightPos - viewPos;
    const float distToL = length( vToL );
    const float3 dirToL = vToL / distToL;
	// attenuation
    const float att = attConst + attLin * distToL + attQuad * ( distToL * distToL );
    const float luminosity = 1.f / att;
	// diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * luminosity * max( 0.f, dot( dirToL, viewN ) );
    // reflected light vector
    const float3 w = viewN * dot( vToL, viewN );
    const float3 r = w * 2.f - vToL;
	// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    // multiplying by luminosity because we are using point light here
    const float3 specular = luminosity * ( diffuseColor * diffuseIntensity ) *
                            specularIntensity * pow( max( 0.f, dot( normalize( -r ), normalize( viewPos ) ) ), specularPower );
	// final color
    return float4( saturate( ( diffuse + ambient ) * tex.Sample( splr, tc ).rgb + specular ), 1.f );
}