#include "CommonOps.hlsli"
#include "CommonLightOps.hlsli"
#include "CommonPointLightOps.hlsli"
#include "CommonShadowOps_PO.hlsli"

Texture2D tex : register( t0 );
Texture2D specTex : register( t1 );

SamplerState splr : register( s0 );

cbuffer SpecCBuff : register( b1 )
{
    bool hasGloss;
    bool useSpecMap;
    float3 specColor;
    float specWeight;
    float specGloss;
};

float4 main( float3 viewPos : Position, float3 viewN : Normal, float2 tc : TexCoord, float4 spos : ShadowPosition ) : SV_Target
{
    float3 specular;
    float3 diffuse;
    const float shadowLevel = shadow(spos);
    if (shadowLevel != 0.0f)
    {
        viewN = normalize( viewN );
        // fragment to light vector data
        const LightVectorData lightVec = calc_light_vector_data( viewLightPos, viewPos );
	    // attenuation
        const float luminosity = calc_luminosity( attConst, attLin, attQuad, lightVec.distToL );
	    // diffuse intensity
        diffuse = calc_diffuse( diffuseColor, diffuseIntensity, luminosity, lightVec.dirToL, viewN );
        // reflected light vector
    
        float4 sampledSpec = specTex.Sample( splr, tc );
        float3 specularColor;
        if( useSpecMap )
        {
            specularColor = sampledSpec.rgb;
        }
        else
        {
            specularColor = specColor;
        }
        float specularPower = specGloss;
        if( hasGloss )
        {
            specularPower = pow( 2.f, sampledSpec.a * 13.f );
        }
        specular = calc_specular( diffuseColor * specularColor, specWeight, viewN, lightVec.vToL, viewPos, luminosity, specularPower );
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