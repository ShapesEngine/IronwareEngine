#include "CommonPointLightOps.hlsli"
#include "CommonLightOps.hlsli"
#include "CommonOps.hlsli"

cbuffer ObjectCBuf
{
    float3 materialColor;
    float3 specularColor;
    float specularWeight;
    float specularGloss;
};

float4 main( float3 viewPos : Position, float3 viewN : Normal ) : SV_Target
{
    viewN = normalize( viewN );
	// fragment to light vector data
    const LightVectorData lightVec = calc_light_vector_data( viewLightPos, viewPos );
	// attenuation
    const float luminosity = calc_luminosity( attConst, attLin, attQuad, lightVec.distToL );
	// diffuse intensity
    const float3 diffuse = calc_diffuse( diffuseColor, diffuseIntensity, luminosity, lightVec.dirToL, viewN );
	// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    const float3 specular = calc_specular( diffuseColor * diffuseIntensity * specularColor, specularWeight, viewN, lightVec.vToL, viewPos, luminosity, specularGloss );
	// final color
    return float4( saturate( diffuse + ambient) * materialColor + specular, 1.f );
}