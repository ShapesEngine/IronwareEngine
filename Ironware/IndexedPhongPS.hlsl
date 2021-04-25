#include "CommonPointLightOps.hlsli"
#include "CommonLightOps.hlsli"
#include "CommonOps.hlsli"

cbuffer ObjectCBuf
{
    float3 materialColors[6];
    float padding;
    float specularIntensity;
    float specularPower;
};


float4 main( float3 viewPos : Position, float3 viewN : Normal, uint tid : SV_PrimitiveID ) : SV_Target
{
    viewN = normalize( viewN );
	// fragment to light vector data
    const LightVectorData lightVec = calc_light_vector_data( viewLightPos, viewPos );
	// attenuation
    const float luminosity = calc_luminosity( attConst, attLin, attQuad, lightVec.distToL );
	// diffuse intensity
    const float3 diffuse = calc_diffuse( diffuseColor, diffuseIntensity, luminosity, lightVec.dirToL, viewN );
	// multiplying by luminosity because we are using point light here
    const float3 specular = calc_specular( diffuseColor, specularIntensity, viewN, lightVec.vToL, viewPos, luminosity, specularPower );  
	// final color
    return float4( saturate( ( diffuse + ambient + specular ) * materialColors[( tid / 2 ) % 6] ), 1.f );
}