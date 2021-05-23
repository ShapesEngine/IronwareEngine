#include "CommonPointLightOps.hlsli"
#include "CommonLightOps.hlsli"
#include "CommonOps.hlsli"
#include "CommonShadowOps_PO.hlsli"

cbuffer ObjectCBuf : register( b1 )
{
    float3 materialColor;
    float3 specularColor;
    float specularWeight;
    float specularGloss;
};

float4 main( float3 viewPos : Position, float3 viewN : Normal, float4 spos : ShadowPosition ) : SV_Target
{
    float3 diffuse;
    float3 specular;
    
    const float shadowLevel = shadow( spos );
    if( shadowLevel != 0.0f )
    {
        
        viewN = normalize( viewN );
	    // fragment to light vector data
        const LightVectorData lightVec = calc_light_vector_data( viewLightPos, viewPos );
	    // attenuation
        const float luminosity = calc_luminosity( attConst, attLin, attQuad, lightVec.distToL );
	    // diffuse intensity
        diffuse = calc_diffuse( diffuseColor, diffuseIntensity, luminosity, lightVec.dirToL, viewN );
	    // calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
        specular = calc_specular( diffuseColor * diffuseIntensity * specularColor, specularWeight, viewN, lightVec.vToL, viewPos, luminosity, specularGloss );
        // scale by shadow level
        diffuse *= shadowLevel;
        specular *= shadowLevel;
    }
    else
    {
        diffuse = specular = 0.f;
    }
    // final color
    return float4( saturate( diffuse + ambient ) * materialColor + specular, 1.f );
}