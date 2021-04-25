// for global dynamic light
#include "CommonPointLightOps.hlsli"
#include "CommonLightOps.hlsli"
#include "CommonOps.hlsli"

// for object color
cbuffer ObjectCBuf
{
    float specularIntensity;
    float specularPower;
};

float4 main( float3 viewPos : Position, float3 viewN : Normal, float3 color : Color ) : SV_Target
{
    viewN = normalize( viewN );
    // fragment to light vector data
    const LightVectorData lightVec = calc_light_vector_data( viewLightPos, viewPos );
	// attenuation
    const float luminosity = calc_luminosity( attConst, attLin, attQuad, lightVec.distToL );
	// diffuse intensity
    const float3 diffuse = calc_diffuse( diffuseColor, diffuseIntensity, luminosity, lightVec.dirToL, viewN );
    const float3 specular = calc_specular( diffuseColor, specularIntensity, viewN, lightVec.vToL, viewPos, luminosity, specularPower );
	// final color
    return float4( saturate( ( diffuse + ambient + specular ) * color ), 1.f );
}