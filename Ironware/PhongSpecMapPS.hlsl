#include "CommonOps.hlsli"
#include "CommonLightOps.hlsli"
#include "CommonPointLightOps.hlsli"

Texture2D tex;
Texture2D specTex;

SamplerState splr;

cbuffer SpecCBuff
{
    float specPower;
    float specMapWeight;
    bool hasGloss;
};

float4 main( float3 viewPos : Position, float3 viewN : Normal, float2 tc : TexCoord ) : SV_Target
{
    viewN = normalize( viewN );
    // fragment to light vector data
    const LightVectorData lightVec = calc_light_vector_data( viewLightPos, viewPos );
	// attenuation
    const float luminosity = calc_luminosity( attConst, attLin, attQuad, lightVec.distToL );
	// diffuse intensity
    const float3 diffuse = calc_diffuse( diffuseColor, diffuseIntensity, luminosity, lightVec.dirToL, viewN );
    // reflected light vector
    
    float4 sampledSpec = specTex.Sample( splr, tc );
    float3 specularColor = sampledSpec.rgb;
    float specularPower = specPower;
    if( hasGloss )
    {
        specularPower = pow( 2.f, sampledSpec.a * 13.f );
    }
    const float3 specular = calc_specular( specularColor, 1.f, viewN, lightVec.vToL, viewPos, luminosity, specularPower );
	// final color
    return float4( saturate( ( diffuse + ambient ) * tex.Sample( splr, tc ).rgb + specular ), 1.f );
}