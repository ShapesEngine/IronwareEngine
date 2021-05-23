#include "CommonOps.hlsli"
#include "CommonLightOps.hlsli"

#include "CommonPointLightOps.hlsli"
#include "CommonShadowOps_PO.hlsli"

cbuffer ObjectCBuf : register( b1 )
{
    float3 specularColor;
    float specularWeight;
    float specularGloss;
};

Texture2D tex : register( t0 );

SamplerState splr : register( s0 );

float4 main( float3 viewFragPos : Position, float3 viewNormal : Normal, float2 tc : Texcoord, float4 spos : ShadowPosition ) : SV_Target
{
    float3 diffuse;
    float3 specular;
    
    // shadow map test
    const float shadowLevel = shadow( spos );
    if( shadowLevel != 0.0f )
    {
        // renormalize interpolated normal
        viewNormal = normalize( viewNormal );
	    // fragment to light vector data
        const LightVectorData lv = calc_light_vector_data( viewLightPos, viewFragPos );
	    // attenuation
        const float att = calc_luminosity( attConst, attLin, attQuad, lv.distToL );
	    // diffuse
        diffuse = calc_diffuse( diffuseColor, diffuseIntensity, att, lv.dirToL, viewNormal );
	    // specular
        specular = calc_specular( diffuseColor * diffuseIntensity * specularColor, specularWeight, viewNormal, lv.vToL, viewFragPos, att, specularGloss );
        // scale by shadow level
        diffuse *= shadowLevel;
        specular *= shadowLevel;
    }
    else
    {
        diffuse = specular = 0.f;
    }
	// final color
    return float4( saturate( ( diffuse + ambient ) * tex.Sample( splr, tc ).rgb + specular ), 1.f );
}