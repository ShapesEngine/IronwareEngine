#include "CommonPointLightOps.hlsli"
#include "CommonLightOps.hlsli"
#include "CommonOps.hlsli"

cbuffer ObjectCBuf
{
    float4 materialColor;
    float4 specularColor;
    float specularPower;
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
    const float4 specular = calc_specular( specularColor.rgb, 1.f, viewN, lightVec.vToL, viewPos, luminosity, specularPower ).rgbr;
	// final color
    return saturate( float4( diffuse + ambient, 1.f ) * materialColor + specular );
}