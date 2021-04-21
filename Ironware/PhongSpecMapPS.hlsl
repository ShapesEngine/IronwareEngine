Texture2D tex;
Texture2D specTex;

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

float4 main( float3 viewPos : Position, float3 viewN : Normal, float2 tc : TexCoord ) : SV_Target
{
    // fragment to light vector data
    const float3 vToL = lightPos - viewPos;
    const float distToL = length( vToL );
    const float3 dirToL = vToL / distToL;
	// attenuation
    const float att = attConst + attLin * distToL + attQuad * ( distToL * distToL );
    const float luminosity = 1.f / att;
	// diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * luminosity * max( 0.f, dot( dirToL, viewN ) ) * (float3)tex.Sample( splr, tc );
    // reflected light vector
    const float3 w = viewN * dot( vToL, viewN );
    const float3 r = w * 2.f - vToL;
	// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    // multiplying by luminosity because we are using point light here
    float4 sampledSpec = specTex.Sample( splr, tc );
    float3 specularColor = sampledSpec.rgb;
    float specularExpPower = pow( 2.f, sampledSpec.a * 13.f );
    const float3 specular = luminosity * ( diffuseColor * diffuseIntensity ) * pow( max( 0.f, dot( normalize( -r ), normalize( viewPos ) ) ), specularExpPower );
	// final color
    return float4( saturate( ( diffuse + ambient ) * tex.Sample( splr, tc ).rgb + specular * specularColor ), 1.f );
}