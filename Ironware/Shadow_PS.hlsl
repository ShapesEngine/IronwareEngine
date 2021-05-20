#include "CommonOps.hlsli"
#include "CommonLightOps.hlsli"

#include "CommonPointLightOps.hlsli"

cbuffer ObjectCBuf
{
    float3 specularColor;
    float specularWeight;
    float specularGloss;
};

Texture2D tex;
Texture2D smap : register( t3 );

SamplerState splr;
SamplerState ssam;

float4 main( float3 viewFragPos : Position, float3 viewNormal : Normal, float2 tc : Texcoord, float4 spos : ShadowPosition ) : SV_Target
{
    float3 diffuse;
    float3 specular;
    
    // shadow map test
    spos.xyz = spos.xyz / spos.w;
    if( smap.Sample( ssam, spos.xy ).r > spos.z )
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
    }
    else
    {
        diffuse = specular = float3( 0.0f, 0.0f, 0.0f );
    }
	// final color
    return float4( saturate( ( diffuse + ambient ) * tex.Sample( splr, tc ).rgb + specular ), 1.0f );
}